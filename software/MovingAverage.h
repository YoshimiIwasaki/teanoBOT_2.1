
#ifndef MOVINGAVERAGE_H
#define MOVINGAVERAGE_H

template <typename T>
class MovingAverage {
public:
    MovingAverage(int times = 20);
    T operator() (T value);
    ~MovingAverage();
private:
    T *_ringBuffer;
    float _result;
    int _times;
    int _count;
    
    T smooth(T data);
};

template <typename T>
class AnglesMovingAverage {
public:
    AnglesMovingAverage(int times = 20, bool useRadian = false);
    T operator() (T value);
    ~AnglesMovingAverage();
private:
    MovingAverage<float> xComponentAverage;
    MovingAverage<float> yComponentAverage;
    bool _useRadian;
    
    T smooth(T angle);
};

#endif /* MOVINGAVERAGE_H */
