// Copyright (c) 2020 DGB. All rights reserved.

#include "Menu.h"

namespace menu
{
    Menu::Menu(const HANDLE& hConsole_, std::string_view name_) :
            hConsole{ hConsole_ },
            menuName{ name_ },
            position{ 0 },
            tooltip{ "Esc - exit" }
    {
        ShowCursor(false);
    }

    void Menu::AddItem(std::string_view name, action_t action)
    {
        menuItems.push_back(MenuItem(name, action));
    }

    void Menu::GetInput()
    {
        if (menuItems.empty())
            return;

        system("cls");
        Initialize();
        // initial item
        SelectItem(0);

        // init timer thread
        timer.InitializeThread(*this);

        // current key code
        auto key = 0;

        // while not pressed escape button
        while (key != static_cast<int>(Keys::Escape))
        {
            // get pressed key
            key = _getch();
            if (key == static_cast<int>(Keys::Enter))
            {
                { // lock_guard scope
                    // locks console mutex
                    std::lock_guard <std::mutex> guard(lock);
                    system("cls");
                    ShowCursor(true);

                    // triggers action of an item
                    menuItems[position].action();
                    system("pause");

                    // repaint menu after end of the action
                    system("cls");
                    ShowCursor(false);
                    ShowMenu();
                }
                SelectItem(position);
            }
            if (key != static_cast<int>(Keys::Arrow))
                continue;
            key = _getch();
            const auto selected = position;

            if (key == static_cast<int>(Keys::ArrowUp))        ++position;
            else if (key == static_cast<int>(Keys::ArrowDown)) --position;

            // goes to last item
            if (position < 0)
                position = menuItems.size() - 1;
                // goes to first item
            else if (position > menuItems.size() - 1)
                position = 0;

            // selection
            if (selected != position)
                SelectItem(position);
        }
    }

    void Menu::ChangeName(std::string_view name)
    {
        menuName = name;
    }

    void Menu::Initialize()
    {
        ShowTime();
        ShowTooltip();
        CONSOLE_SCREEN_BUFFER_INFO screenBuffer{};
        // remember positions of items
        for (auto& item : menuItems)
        {
            GetConsoleScreenBufferInfo(hConsole, &screenBuffer);
            item.position = screenBuffer.dwCursorPosition;
            std::cout << item.name << std::endl;
        }
    }

    void Menu::SelectItem(int position)
    {
        // locks console mutex
        std::lock_guard <std::mutex> guard(lock);
        system("color 0F");
        SetConsoleCursorPosition(hConsole, menuItems[position].position);
        // change color
        SetConsoleTextAttribute(hConsole, (WORD)((15 << 4) | 0));
        std::cout << menuItems[position].name;
        SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 15));
        // return color

        SetCursorToEnd();
    }

    void Menu::SetCursorToEnd()
    {
        SetConsoleCursorPosition(hConsole, menuItems[menuItems.size() - 1].position);
        std::cout << std::endl;
    }

    void Menu::ShowMenu() const
    {
        ShowTime();
        ShowTooltip();
        ShowItems();
    }

    void Menu::ShowTime() const
    {
        SYSTEMTIME st;
        GetLocalTime(&st);
        printf("%d-%02d-%02d %02d:%02d:%02d",
               st.wYear,
               st.wMonth,
               st.wDay,
               st.wHour,
               st.wMinute,
               st.wSecond);
        std::cout << std::endl;
    }

    void Menu::ShowTooltip() const
    {
        std::cout << tooltip << std::endl << std::endl << menuName << std::endl;
    }

    void Menu::ShowItems() const
    {
        for (auto& item : menuItems)
            std::cout << item.name << std::endl;
    }

    void Menu::ShowCursor(bool visibility)
    {
        // making cursor visible/invisible
        CONSOLE_CURSOR_INFO structCursorInfo;
        GetConsoleCursorInfo(hConsole, &structCursorInfo);
        structCursorInfo.bVisible = visibility;
        SetConsoleCursorInfo(hConsole, &structCursorInfo);
    }

    void Menu::Timer::InitializeThread(Menu& menu)
    {
        isWorking = true;
        lastCheckedTime = std::chrono::system_clock::now() + std::chrono::seconds{ 1 };
        // moves CheckTime function to the another thread using lambda expression
        timeThread = std::thread{ [&menu, this]() { menu.CheckTime(*this); } };
    }

    void Menu::CheckTime(Timer& timer)
    {
        COORD timePosition{ 0,0 };
        // check time every 50ms
        while (timer.isWorking)
        {
            if (timer.lastCheckedTime <= std::chrono::system_clock::now())
            {
                // locks console mutex
                std::lock_guard <std::mutex> guard(lock);
                SetConsoleCursorPosition(hConsole, timePosition);
                ShowTime();
                SetCursorToEnd();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds{ 50 });
        }
    }

    Menu::~Menu()
    {
        // close timer thread
        timer.isWorking = false;
        if (timer.timeThread.joinable())
            timer.timeThread.join();
    }
}

