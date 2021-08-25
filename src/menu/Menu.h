// Copyright (c) 2020 DGB. All rights reserved.

#include <Windows.h>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <chrono>
#include <thread>
#include <mutex>

namespace menu
{
    // menu class
    class Menu
    {
    public:
        using action_t = std::function<void()>;

        // constructors
        Menu(const HANDLE&, std::string_view name = "Menu:");
        Menu(const Menu&) = default;
        Menu(Menu&&) = default;

        // = operators
        Menu& operator= (const Menu&) = default;
        Menu& operator= (Menu&&) = default;

        // destructor
        ~Menu();

    public:
        // changes menu name
        void ChangeName(std::string_view);
        // adds an menu item
        void AddItem(std::string_view, action_t);
        // starts up menu
        void GetInput();

    protected:
        // intializes menu
        void Initialize();
        // selects item
        void SelectItem(int position);
        // sets console cursor to the end
        void SetCursorToEnd();
        // shows menu
        void ShowMenu() const;
        // shows time
        void ShowTime() const;
        // shows tooltip
        void ShowTooltip() const;
        // shows items
        void ShowItems() const;
        // shows/hides console cursor
        void ShowCursor(bool);

    protected:
        // struct for items of menu
        struct MenuItem
        {
            // constructor
            MenuItem(std::string_view name_, action_t action_, COORD position_ = COORD{})
                    : name{ name_ }, action{ action_ }, position{ position_ }
            {
            }

            // fields
            std::string name;
            action_t action;
            COORD position;
        };

        // timer struct for menu
        struct Timer
        {
            // constructor
            Timer() : isWorking{ false } {}
            // initializes another thread
            void InitializeThread(Menu& menu);

            // thread flag
            bool isWorking;
            // last timer check
            std::chrono::system_clock::time_point lastCheckedTime;
            // time thread
            std::thread timeThread;
        };

    protected:
        // checks time for timer
        void CheckTime(Timer&);
        // menu timer
        Timer timer;
        // mutex for console
        std::mutex lock;

    protected:
        // enumeration for keyborad keys
        enum class Keys {
            Arrow = 224,
            ArrowUp = 80,
            ArrowDown = 72,
            Enter = 13,
            Escape = 27
        };
        // console handles
        HANDLE hConsole;
        // menu position
        int position;
        // menu name
        std::string menuName;
        // menu tooltip
        std::string tooltip;
        // vector of menu items
        std::vector<MenuItem> menuItems;
    };
}