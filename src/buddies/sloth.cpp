#include "../buddy.h"
#include "../buddy_common.h"
#ifdef M5STACK_FIRE
#include <M5Stack.h>
#else
#include <M5StickCPlus.h>
#endif
#include <string.h>

extern TFT_eSprite spr;

namespace sloth {

// Body: (~______~) shaggy round head, sleepy dots for eyes,
//       \\( ~~~ )/ gentle smile, mm……mm big curved claws.
// Slow-motion — tick divisors of 7-8 to honour the legend.

// ─── SLEEP ─── barely alive; huge Z's drift upward
static void doSleep(uint32_t t) {
  static const char* const HANG[5]    = { "            ", " (~______~) ", "( --    -- )", "  \\( ~~~ )/ ", "  mm      mm" };
  static const char* const DROOP[5]   = { "            ", " (~______~) ", "( --    -- )", "  \\( ___ )/ ", "  mm      mm" };
  static const char* const DEEP[5]    = { "            ", " (~______~) ", "(---      --)", "  \\( ___ )/ ", "  mm      mm" };

  const char* const* P[3] = { HANG, DROOP, DEEP };
  static const uint8_t SEQ[] = {
    0,0,0,0,0,0,1,0,
    0,0,0,0,0,2,2,0,
    0,0,0,0,0,1,0,0
  };
  uint8_t beat = (t / 8) % sizeof(SEQ);
  buddyPrintSprite(P[SEQ[beat]], 5, 0, 0xB46A);

  int p1 = (t / 2)     % 14;
  int p2 = (t / 2 + 5) % 14;
  int p3 = (t / 2 + 9) % 14;
  buddySetColor(BUDDY_DIM);
  buddySetCursor(BUDDY_X_CENTER + 20 + (p1 % 3), BUDDY_Y_OVERLAY + 18 - p1);
  buddyPrint("z");
  buddySetCursor(BUDDY_X_CENTER + 25 - (p2 % 2), BUDDY_Y_OVERLAY + 13 - p2);
  buddyPrint("Z");
  buddySetColor(BUDDY_WHITE);
  buddySetCursor(BUDDY_X_CENTER + 22 + (p3 % 3), BUDDY_Y_OVERLAY + 8 - p3 / 2);
  buddyPrint("Z");
}

// ─── IDLE ─── half-lidded; turns head on geologic timescales
static void doIdle(uint32_t t) {
  static const char* const REST[5]    = { "            ", " (~______~) ", "( .      . )", "  \\( ~~~ )/ ", "  mm      mm" };
  static const char* const LOOK_L[5]  = { "            ", " (~______~) ", "(.       . )", "  \\( ~~~ )/ ", "  mm      mm" };
  static const char* const LOOK_R[5]  = { "            ", " (~______~) ", "( .       .)", "  \\( ~~~ )/ ", "  mm      mm" };
  static const char* const BLINK[5]   = { "            ", " (~______~) ", "( -      - )", "  \\( ~~~ )/ ", "  mm      mm" };
  static const char* const YAWN[5]    = { "            ", " (~______~) ", "( o      o )", "  \\(  __  )/ ", "  mm      mm" };

  const char* const* P[5] = { REST, LOOK_L, LOOK_R, BLINK, YAWN };
  static const uint8_t SEQ[] = {
    0,0,0,0,0,0,3,0,
    0,0,0,1,1,0,0,0,3,0,
    0,0,0,2,2,0,0,0,3,4,4,
    0,0,0,3,0
  };
  uint8_t beat = (t / 7) % sizeof(SEQ);
  buddyPrintSprite(P[SEQ[beat]], 5, 0, 0xB46A);
}

// ─── BUSY ─── straining with visible effort; sweat drop crawls down
static void doBusy(uint32_t t) {
  static const char* const GRIP[5]    = { "            ", " (~______~) ", "( >      < )", "  \\(  __  )/ ", "  mm      mm" };
  static const char* const STRAIN[5]  = { "            ", "(~______~)  ", "( v      v )", "  \\(  --  )/ ", " /mm      mm\\" };
  static const char* const THINK[5]   = { "     ?      ", " (~______~) ", "( ^      ^ )", "  \\(  ..  )/ ", "  mm      mm" };
  static const char* const PUFF[5]    = { "   ~~~      ", " (~______~) ", "( -      - )", "  \\(  __  )/ ", "  mm      mm" };

  const char* const* P[4] = { GRIP, STRAIN, THINK, PUFF };
  static const uint8_t SEQ[] = { 0,1,0,1,0,1, 2,2, 0,1,0,1, 3, 2,0,1 };
  uint8_t beat = (t / 5) % sizeof(SEQ);
  buddyPrintSprite(P[SEQ[beat]], 5, 0, 0xB46A);

  // sweat drop crawls slowly downward
  int s = (t * 2) % 14;
  buddySetColor(BUDDY_CYAN);
  buddySetCursor(BUDDY_X_CENTER + 24, BUDDY_Y_OVERLAY + 4 + s);
  buddyPrint(s < 5 ? "," : s < 10 ? ";" : ".");
}

// ─── ATTENTION ─── suddenly, shockingly awake
static void doAttention(uint32_t t) {
  static const char* const ALERT[5]   = { "            ", " (~______~) ", "( O      O )", "  \\(  !!  )/ ", "  mm      mm" };
  static const char* const WIDE[5]    = { "            ", "((~______~))", "( O      O )", "  \\(  !!  )/ ", "  mm      mm" };
  static const char* const REACH[5]   = { " /        \\ ", " (~______~) ", "( O      O )", "  \\(  !!  )/ ", "  mm      mm" };

  const char* const* P[3] = { ALERT, WIDE, REACH };
  static const uint8_t SEQ[] = { 0,1,0,1,0,2,0,1, 0,2,0,1, 0,1,0 };
  uint8_t beat = (t / 4) % sizeof(SEQ);
  int xOff = (SEQ[beat] == 1) ? ((t & 1) ? 1 : -1) : 0;
  buddyPrintSprite(P[SEQ[beat]], 5, 0, 0xB46A, xOff);

  if ((t / 2) & 1) {
    buddySetColor(BUDDY_YEL);
    buddySetCursor(BUDDY_X_CENTER - 6, BUDDY_Y_OVERLAY);
    buddyPrint("!");
  }
  if ((t / 3) & 1) {
    buddySetColor(BUDDY_RED);
    buddySetCursor(BUDDY_X_CENTER + 6, BUDDY_Y_OVERLAY + 4);
    buddyPrint("!");
  }
}

// ─── CELEBRATE ─── a genuine sway; arms actually rise
static void doCelebrate(uint32_t t) {
  static const char* const SWAY_L[5]  = { " /          ", " (~______~) ", "( ^      ^ )", "  \\( ~~~ )/ ", "  mm      mm" };
  static const char* const SWAY_R[5]  = { "          \\ ", " (~______~) ", "( ^      ^ )", "  \\( ~~~ )/ ", "  mm      mm" };
  static const char* const BOTH[5]    = { " /        \\ ", " (~______~) ", "( ^      ^ )", "  \\( ~~~ )/ ", "  mm      mm" };
  static const char* const GRIN[5]    = { "            ", " (~______~) ", "( ^      ^ )", "  \\( www )/ ", "  mm      mm" };

  const char* const* P[4] = { SWAY_L, SWAY_R, BOTH, GRIN };
  static const uint8_t SEQ[] = { 0,1,0,1,2,3,3,2, 0,1,0,1,2,2 };
  static const int8_t Y_SHIFT[] = { 0,0,0,0,-2,-3,-3,-2, 0,0,0,0,-2,-2 };
  uint8_t beat = (t / 4) % sizeof(SEQ);
  buddyPrintSprite(P[SEQ[beat]], 5, Y_SHIFT[beat], 0xB46A);

  // leaf confetti — the sloth's natural habitat
  static const uint16_t cols[] = { BUDDY_GREEN, BUDDY_YEL, BUDDY_GREEN, BUDDY_CYAN, BUDDY_WHITE };
  for (int i = 0; i < 6; i++) {
    int phase = (t * 2 + i * 9) % 22;
    int x = BUDDY_X_CENTER - 34 + i * 13;
    int y = BUDDY_Y_OVERLAY - 4 + phase;
    if (y < 0 || y > BUDDY_Y_BASE + 18) continue;
    buddySetColor(cols[i % 5]);
    buddySetCursor(x, y);
    buddyPrint((i + (int)(t / 2)) & 1 ? "~" : "*");
  }
}

// ─── DIZZY ─── already slow, now spinning with glazed eyes
static void doDizzy(uint32_t t) {
  static const char* const TILT_L[5]  = { "            ", " (~______~) ", "( @      . )", "  \\( ~~~ )/ ", "  mm      mm" };
  static const char* const TILT_R[5]  = { "            ", " (~______~) ", "( .      @ )", "  \\( ~~~ )/ ", "  mm      mm" };
  static const char* const GLAZE[5]   = { "            ", " (~______~) ", "( x      x )", "  \\( ~~v )/ ", "  mm      mm" };
  static const char* const SLUMP[5]   = { "            ", " (~______~) ", "( @      @ )", "  \\(  -- )/ ", "  ~m      m~" };

  const char* const* P[4] = { TILT_L, TILT_R, GLAZE, SLUMP };
  static const uint8_t SEQ[] = { 0,1,0,1, 2,3,2, 0,1,0,1, 3,2,3 };
  static const int8_t X_SHIFT[] = { -2,2,-2,2, 0,0,0, -2,2,-2,2, 0,0,0 };
  uint8_t beat = (t / 4) % sizeof(SEQ);
  buddyPrintSprite(P[SEQ[beat]], 5, 0, 0xB46A, X_SHIFT[beat]);

  static const int8_t OX[] = { 0, 5, 7, 5, 0,-5,-7,-5 };
  static const int8_t OY[] = {-5,-3, 0, 3, 5, 3, 0,-3 };
  uint8_t p1 = t % 8;
  uint8_t p2 = (t + 4) % 8;
  buddySetColor(BUDDY_YEL);
  buddySetCursor(BUDDY_X_CENTER + OX[p1], BUDDY_Y_OVERLAY + 4 + OY[p1]);
  buddyPrint("*");
  buddySetColor(BUDDY_CYAN);
  buddySetCursor(BUDDY_X_CENTER + OX[p2], BUDDY_Y_OVERLAY + 4 + OY[p2]);
  buddyPrint("*");
}

// ─── HEART ─── slow dreamy love; hearts drift up at a slothful pace
static void doHeart(uint32_t t) {
  static const char* const DREAMY[5]  = { "            ", " (~______~) ", "( v      v )", "  \\( ~~~ )/ ", "  mm      mm" };
  static const char* const BLUSH[5]   = { "            ", " (~______~) ", "(#v      v#)", "  \\( ~~~ )/ ", "  mm      mm" };
  static const char* const WINK[5]    = { "            ", " (~______~) ", "( v      - )", "  \\( ~~~ )/ ", "  mm      mm" };

  const char* const* P[3] = { DREAMY, BLUSH, WINK };
  static const uint8_t SEQ[] = { 0,0,1,0,0,0,1,0, 2,0,0,1,0,0,0,2,0 };
  static const int8_t Y_BOB[] = { 0,-1,0,-1,0,0,-1,0, 0,-1,0,0,-1,0,0,-1,0 };
  uint8_t beat = (t / 6) % sizeof(SEQ);
  buddyPrintSprite(P[SEQ[beat]], 5, Y_BOB[beat], 0xB46A);

  buddySetColor(BUDDY_HEART);
  for (int i = 0; i < 4; i++) {
    int phase = (t / 2 + i * 5) % 18;
    int y = BUDDY_Y_OVERLAY + 14 - phase;
    if (y < -2 || y > BUDDY_Y_BASE + 4) continue;
    int x = BUDDY_X_CENTER - 14 + i * 9 + ((phase / 4) & 1) * 2;
    buddySetCursor(x, y);
    buddyPrint("v");
  }
}

}  // namespace sloth

extern const Species SLOTH_SPECIES = {
  "sloth",
  0xB46A,
  { sloth::doSleep, sloth::doIdle, sloth::doBusy, sloth::doAttention,
    sloth::doCelebrate, sloth::doDizzy, sloth::doHeart }
};
