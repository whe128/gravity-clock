#ifndef __BEEP_H
#define __BEEP_H

// Time calculated based on 0.1s intervals
#define BEEP_COUNT      5       // 5

#define BEEP_CYCLE      6       // 6 short beeps, beep 3 times quickly
#define BEEP_TOTAL      40      // Total of 40 beeps in one cycle, repeats n/2 times

void beepSound(void);
void beepStop(void);
void beepControl(void);        // Perform some indicative actions or control strategy

#endif
