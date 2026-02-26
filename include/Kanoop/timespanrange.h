/**
 * @brief An interval of time defined by a start and end TimeSpan offset.
 */
#ifndef TIMESPANRANGE_H
#define TIMESPANRANGE_H
#include <Kanoop/timespan.h>
#include "kanoopcommon.h"

class DateRange;

/**
 * @brief An interval of time defined by a start TimeSpan offset and an end TimeSpan offset.
 *
 * Useful for windowing or slicing time-series data.  All arithmetic operators shift both
 * endpoints by the same delta.  The inner List class provides consolidated() to merge
 * overlapping or adjacent ranges into a minimal non-overlapping set.
 */
class KANOOP_EXPORT TimeSpanRange
{
public:
    /** @brief Default constructor — creates an invalid (default-constructed) range. */
    TimeSpanRange() {}

    /**
     * @brief Construct a range from explicit start and end offsets.
     * @param startOffset Start of the range
     * @param endOffset End of the range
     */
    TimeSpanRange(const TimeSpan& startOffset, const TimeSpan& endOffset) :
        _startOffset(startOffset), _endOffset(endOffset) {}

    /**
     * @brief Construct a range from an absolute QDateTime start and a duration.
     * @param startTime Start time (converted to a millisecond-epoch offset)
     * @param duration Length of the range
     */
    TimeSpanRange(const QDateTime& startTime, const TimeSpan& duration) :
        _startOffset(TimeSpan::fromMilliseconds(startTime.toMSecsSinceEpoch())),
        _endOffset(TimeSpan::fromMilliseconds(startTime.toMSecsSinceEpoch()) + duration) {}

    /**
     * @brief Copy constructor.
     * @param other Range to copy
     */
    TimeSpanRange(const TimeSpanRange& other);

    /**
     * @brief Construct a range centred on a midpoint with a given total duration.
     * @param midpoint Centre of the range
     * @param duration Total length of the range
     * @return TimeSpanRange spanning [midpoint − duration/2, midpoint + duration/2]
     */
    static TimeSpanRange fromMidpoint(const TimeSpan& midpoint, const TimeSpan& duration);

    /**
     * @brief Construct a TimeSpanRange from a DateRange.
     * @param dateRange Source date range
     * @return Equivalent TimeSpanRange expressed as millisecond-epoch offsets
     */
    static TimeSpanRange fromDateRange(const DateRange& dateRange);

    /**
     * @brief Construct a range from a start offset and a duration.
     * @param startOffset Start of the range
     * @param duration Length of the range
     * @return TimeSpanRange spanning [startOffset, startOffset + duration]
     */
    static TimeSpanRange fromStartAndDuration(const TimeSpan& startOffset, const TimeSpan& duration);

    /** @brief Copy assignment. */
    TimeSpanRange& operator=(const TimeSpanRange& other);

    /**
     * @brief Shift both endpoints forward by a TimeSpan delta.
     * @param other Amount to add to both endpoints
     * @return Shifted range
     */
    TimeSpanRange operator+(const TimeSpan& other) const;

    /**
     * @brief Shift both endpoints backward by a TimeSpan delta.
     * @param other Amount to subtract from both endpoints
     * @return Shifted range
     */
    TimeSpanRange operator-(const TimeSpan& other) const;

    /** @brief Add-assign a TimeSpan delta to both endpoints. */
    void operator+=(const TimeSpan& other);
    /** @brief Subtract-assign a TimeSpan delta from both endpoints. */
    void operator-=(const TimeSpan& other);

    /** @brief Equality comparison. */
    bool operator==(const TimeSpanRange& other) const;
    /** @brief Inequality comparison. */
    bool operator!=(const TimeSpanRange& other) const;

    /**
     * @brief Return the start offset.
     * @return Start TimeSpan
     */
    TimeSpan startOffset() const { return _startOffset; }

    /**
     * @brief Set the start offset.
     * @param value New start TimeSpan
     */
    void setStartOffset(const TimeSpan& value) { _startOffset = value; }

    /**
     * @brief Return the end offset.
     * @return End TimeSpan
     */
    TimeSpan endOffset() const { return _endOffset; }

    /**
     * @brief Set the end offset.
     * @param value New end TimeSpan
     */
    void setEndOffset(const TimeSpan& value) { _endOffset = value; }

    /**
     * @brief Return the duration of this range (end − start).
     * @return Duration TimeSpan
     */
    TimeSpan duration() const { return _endOffset - _startOffset; }

    /**
     * @brief Return the midpoint of this range.
     * @return Midpoint TimeSpan
     */
    TimeSpan midpoint() const { return _startOffset + (duration() / 2); }

    /**
     * @brief Test whether a time offset falls within this range (inclusive).
     * @param time TimeSpan offset to test
     * @return true if startOffset ≤ time ≤ endOffset
     */
    bool contains(const TimeSpan& time) const { return time >= _startOffset && time <= _endOffset; }

    /**
     * @brief Test whether another range is entirely contained within this one.
     * @param other Range to test
     * @return true if both endpoints of other lie within this range
     */
    bool contains(const TimeSpanRange& other) const { return contains(other.startOffset()) && contains(other.endOffset()); }

    /**
     * @brief Return the portion of another range that overlaps this range.
     * @param other Range to intersect with
     * @return Intersecting sub-range
     */
    TimeSpanRange subRange(const TimeSpanRange& other) const;

    /**
     * @brief Return the union of this range and another, spanning both.
     * @param other Range to merge with
     * @return Merged range from the earlier start to the later end
     */
    TimeSpanRange merged(const TimeSpanRange& other) const;

    /**
     * @brief Return the union of two ranges, spanning both.
     * @param a First range
     * @param b Second range
     * @return Merged range
     */
    static TimeSpanRange merged(const TimeSpanRange& a, const TimeSpanRange& b) { return a.merged(b); }

    /**
     * @brief Format this range as a human-readable string.
     * @return String representation of the range
     */
    QString toString() const;

    /**
     * @brief Return true if both endpoints are valid TimeSpan values.
     * @return true if start and end offsets are both valid
     */
    bool isValid() const { return _startOffset.isValid() && _endOffset.isValid(); }

    /**
     * @brief A QList of TimeSpanRange objects with a consolidated() helper.
     */
    class List : public QList<TimeSpanRange>
    {
    public:
        /**
         * @brief Merge overlapping or adjacent ranges into a minimal sorted non-overlapping list.
         * @return New List with all overlapping ranges merged
         */
        List consolidated() const
        {
            List result;
            List sorted = *this;
            std::sort(sorted.begin(), sorted.end(), [](const TimeSpanRange& a, const TimeSpanRange& b) { return a.startOffset() < b.startOffset(); });

            if(sorted.count() < 2) {
                return sorted;
            }

            TimeSpanRange current = first();

            for (int i = 1; i < size(); ++i) {
                const TimeSpanRange& next = (*this)[i];

                if (current.endOffset() >= next.startOffset()) {
                    current = TimeSpanRange(
                                  current.startOffset(),
                                  std::max(current.endOffset(), next.endOffset())
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
    TimeSpan _startOffset;
    TimeSpan _endOffset;
};

#endif // TIMESPANRANGE_H
