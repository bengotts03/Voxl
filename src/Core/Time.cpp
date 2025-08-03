//
// Created by Ben Gotts on 03/08/2025.
//

#include "Time.h"

double Time::NormalTime = 0;
double Time::DeltaTime = 0;
double Time::PreviousNormalTime = 0;

Time::Time()= default;

void Time::Update() {
    NormalTime = glfwGetTime();

    DeltaTime = NormalTime - PreviousNormalTime;

    PreviousNormalTime = NormalTime;
}
