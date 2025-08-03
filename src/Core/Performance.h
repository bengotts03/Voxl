//
// Created by Ben Gotts on 03/08/2025.
//

#ifndef FPS_H
#define FPS_H

class Performance {
public:
    Performance();

    static float FPS;
    static float AverageFPS;

    static float MS;

    static void Update();
private:
    static double _timeDifference;
    static unsigned int _fpsCount;
    static double _previousTime;
};

#endif //FPS_H
