/**
 * @brief A time range defined by a start and end QDateTime.
 *
 * DateRange represents a contiguous span of time. It supports arithmetic with TimeSpan,
 * overlap/containment queries, and consolidation of overlapping ranges via DateRange::List.
 */
#ifndef DATERANGE_H
#define DATERANGE_H
#include <QDateTime>

#include <Kanoop/timespan.h>
#include "kanoopcommon.h"

/**
 * @brief Represents a contiguous date/time range with start and end timestamps.
 */
class KANOOP_EXPORT DateRange
{
public:
    /** @brief Default constructor — creates an invalid (empty) date range. */
    DateRange() {}

    /**
     * @brief Construct a DateRange from explicit start and end times.
     * @param startTime Range begin timestamp
     * @param endTime Range end timestamp
     */
    DateRange(const QDateTime& startTime, const QDateTime& endTime) :
        _startTime(startTime), _endTime(endTime) {}

    /**
     * @brief Construct a DateRange from a start time and duration.
     * @param startTime Range begin timestamp
     * @param duration Length of the range
     */
    DateRange(const QDateTime& startTime, const TimeSpan& duration) :
        _startTime(startTime), _endTime(startTime.addMSecs(duration.totalMilliseconds())) {}

    /** @brief Copy constructor. */
    DateRange(const DateRange& other);

    /**
     * @brief Create a DateRange centred on a midpoint with the given duration.
     * @param midpoint Centre of the range
     * @param duration Total length of the range
     * @return DateRange symmetrically straddling midpoint
     */
    static DateRange fromMidpoint(const QDateTime& midpoint, const TimeSpan& duration);

    /** @brief Assignment operator. */
    DateRange& operator=(const DateRange& other);

    /**
     * @brief Shift both endpoints forward by a TimeSpan.
     * @param other Amount to add
     * @return New shifted DateRange
     */
    DateRange operator+(const TimeSpan& other) const;

    /**
     * @brief Shift both endpoints backward by a TimeSpan.
     * @param other Amount to subtract
     * @return New shifted DateRange
     */
    DateRange operator-(const TimeSpan& other) const;

    /** @brief Shift both endpoints forward by a TimeSpan in place. */
    void operator+=(const TimeSpan& other);
    /** @brief Shift both endpoints backward by a TimeSpan in place. */
    void operator-=(const TimeSpan& other);

    /** @brief Equality comparison. */
    bool operator==(const DateRange& other) const;
    /** @brief Inequality comparison. */
    bool operator!=(const DateRange& other) const;

    /**
     * @brief Return the range start time.
     * @return Start QDateTime
     */
    QDateTime startTime() const { return _startTime; }

    /**
     * @brief Set the range start time.
     * @param value New start QDateTime
     */
    void setStartTime(const QDateTime& value) { _startTime = value; }

    /**
     * @brief Return the range end time.
     * @return End QDateTime
     */
    QDateTime endTime() const { return _endTime; }

    /**
     * @brief Set the range end time.
     * @param value New end QDateTime
     */
    void setEndTime(const QDateTime& value) { _endTime = value; }

    /**
     * @brief Return the absolute duration of the range.
     * @return TimeSpan between start and end
     */
    TimeSpan duration() const { return TimeSpan::absDiff(_startTime, _endTime); }

    /**
     * @brief Return the midpoint of the range.
     * @return QDateTime at the centre of the range
     */
    QDateTime midpoint() const { return _startTime.addMSecs((duration() / 2).totalMilliseconds()); }

    /**
     * @brief Test whether a timestamp falls within the range (inclusive).
     * @param time Timestamp to test
     * @return true if time >= startTime && time <= endTime
     */
    bool contains(const QDateTime& time) const { return time >= _startTime && time <= _endTime; }

    /**
     * @brief Test whether another DateRange is entirely contained within this one.
     * @param other Range to test
     * @return true if other is fully within this range
     */
    bool contains(const DateRange& other) const { return contains(other.startTime()) && contains(other.endTime()); }

    /**
     * @brief Test whether another DateRange is intersects with this one.
     * @param other Range to test
     * @return true if other is fully within this range
     */
    bool intersects(const DateRange& other) const;

    /**
     * @brief Return the intersection of this range with another.
     * @param other Range to intersect with
     * @return Overlapping sub-range, or an invalid DateRange if no overlap
     */
    DateRange subRange(const DateRange& other) const;

    /**
     * @brief Return the union of this range and another.
     * @param other Range to merge with
     * @return DateRange spanning both ranges
     */
    DateRange merged(const DateRange& other) const;

    /**
     * @brief Return the union of two ranges (static convenience overload).
     * @param a First range
     * @param b Second range
     * @return DateRange spanning both
     */
    static DateRange merged(const DateRange& a, const DateRange& b) { return a.merged(b); }

    /**
     * @brief Format the range as a human-readable string.
     * @return String representation of start and end times
     */
    QString toString() const;

    /**
     * @brief Check whether both endpoints are valid QDateTime values.
     * @return true if startTime and endTime are valid
     */
    bool isValid() const { return _startTime.isValid() && _endTime.isValid(); }

    /**
     * @brief A sortable, consolidatable list of DateRange objects.
     */
    class List : public QList<DateRange>
    {
    public:
        /**
         * @brief Return a new list with overlapping/adjacent ranges merged.
         * @return Consolidated list sorted by start time
         */
        List consolidated() const
        {
            List result;
            List sorted = *this;
            std::sort(sorted.begin(), sorted.end(), [](const DateRange& a, const DateRange& b) { return a.startTime() < b.startTime(); });

            if(sorted.count() < 2) {
                return sorted;
            }

            DateRange current = first();

            for (int i = 1; i < size(); ++i) {
                const DateRange& next = (*this)[i];

                if (current.endTime() >= next.startTime()) {
                    current = DateRange(
                                  current.startTime(),
                                  std::max(current.endTime(), next.endTime())
                                  );
                }
                else {
                    result.append(current);
                    current = next;
                }
            }
            result.append(current);

            return result;
        }
    };

private:
    QDateTime _startTime;
    QDateTime _endTime;
};

#endif // DATERANGE_H
