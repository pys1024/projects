#pragma once

#include "../Page.h"

namespace Page {
    class MyTest : public PageBase {
    public:
        MyTest();
        ~MyTest();

        void onViewLoad();

    }; // class MyTest
}