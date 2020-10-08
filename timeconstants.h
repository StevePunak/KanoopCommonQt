#ifndef __TIMECONSTANTS_H
#define __TIMECONSTANTS_H


#define __NanosecondsPerMicrosecond                  (uint64_t)1000L
#define __NanosecondsPerMillisecond                  (uint64_t)1000000L
#define __NanosecondsPerSecond                       (uint64_t)1000000000L
#define __NanosecondsPerMinute                       (uint64_t)__NanosecondsPerSecond * 60L
#define __NanosecondsPerHour                         (uint64_t)__NanosecondsPerMinute * 60L
#define __NanosecondsPerDay                          (uint64_t)__NanosecondsPerHour * 24L

#define __MicrosecondsPerMillisecond                 (uint64_t)1000L
#define __MicrosecondsPerSecond                      (uint64_t)1000000L
#define __MicrosecondsPerMinute                      (uint64_t)__MicrosecondsPerSecond * 60L
#define __MicrosecondsPerHour                        (uint64_t)__MicrosecondsPerMinute * 60L
#define __MicrosecondsPerDay                         (uint64_t)__MicrosecondsPerHour * 24L

#define __MillisecondsPerSecond                      (uint64_t)1000L
#define __MillisecondsPerMinute                      (uint64_t)__MillisecondsPerSecond * 60L
#define __MillisecondsPerHour                        (uint64_t)__MillisecondsPerMinute * 60L
#define __MillisecondsPerDay                         (uint64_t)__MillisecondsPerHour * 24L

#define __SecondsPerMinute                          (uint64_t)60L
#define __SecondsPerHour                            (uint64_t)__SecondsPerMinute * 60L
#define __SecondsPerDay                             (uint64_t)__SecondsPerHour * 24L

#define __MinutesPerHour                            (uint64_t)60L
#define __MinutesPerDay                             (uint64_t)__MinutesPerHour * 24L

#define __HoursPerDay                               (uint64_t)24L

#define __DaysPerYear                               365.242;


#endif //` __TIMECONSTANTS_H


