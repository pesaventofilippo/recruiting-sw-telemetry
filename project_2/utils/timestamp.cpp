#include "timestamp.h"
using namespace std;


uint64_t msTimestamp() {
    // Return the current unix timestamp in milliseconds
    using namespace chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
