#ifndef TIMER_HPP
#define TIMER_HPP

class TimerUtil {
private:
    bool isActive = false;
    bool timerStarted = false;
    double startTime = 0.0;
    double elapsedSeconds = 0.0;

public:
    TimerUtil() = default;

    void start();
    void stop();
    void reset();
    void update();

    bool getIsActive() const { return isActive; }
    bool getTimerStarted() const { return timerStarted; }
    double getElapsedSeconds() const { return elapsedSeconds; }
};

#endif // TIMER_HPP
