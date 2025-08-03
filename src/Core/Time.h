//
// Created by Ben Gotts on 03/08/2025.
//

#ifndef TIME_H
#define TIME_H

#include <GLFW/glfw3.h>

class Time {
public:
    Time();

    static double NormalTime;
    static double PreviousNormalTime;
    static double DeltaTime;

    static void Update();
private:
};

#endif //TIME_H