#ifndef __DATA_TYPE__
#define __DATA_TYPE__

// Data types used by multiple classes
typedef struct {
	float f_match_rate;
	float f_notmatch_rate;
} MatchRate;

typedef enum 
{
    LEFT = 0,
    RIGHT,
    LIMIT = 2
} TwoWayDirections;

typedef enum
{
    NOTSTART = 0,
    INPROGRESS = 1,
    COMPLETE = 2
} ThreadStatus;

#endif
