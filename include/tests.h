#ifndef __TESTS_H__
#define __TESTS_H__
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Use high-resolution timing if available
#ifdef _POSIX_C_SOURCE
#include <sys/time.h>
#define USE_HIGH_RES_TIMER 1
#else
#define USE_HIGH_RES_TIMER 0
#endif

// Epic test color codes for mythological visibility
#define TEST_COLOR_RESET      "\033[0m"
#define TEST_COLOR_GREEN      "\033[32m"
#define TEST_COLOR_RED        "\033[31m"
#define TEST_COLOR_BLUE       "\033[34m"
#define TEST_COLOR_WHITE      "\033[37m"
#define TEST_COLOR_YELLOW     "\033[33m"
#define TEST_COLOR_BOLD_WHITE "\033[1;37m"
#define TEST_COLOR_PURPLE     "\033[35m"
#define TEST_COLOR_CYAN       "\033[36m"
#define TEST_COLOR_ORANGE     "\033[93m"
#define TEST_COLOR_GRAY       "\033[2m"
#define TEST_COLOR_GOLD       "\033[1;33m"

// Global test counters and timing (to be defined in each test file)
extern int total_tests;
extern int tests_passed;
extern int tests_failed;

// Global timing variables for suite-level timing
static double _test_suite_start_time;
static double _total_test_time = 0.0;
// Loop detection variables
static int _sisyphus_in_loop = 0;
static int _sisyphus_loop_iteration = 0;
// Sisyphus gamification system
static int _sisyphus_beasts_slayed = 0;
static int _sisyphus_trials_conquered = 0;
static int _sisyphus_perfect_streak = 0;
static int _sisyphus_xp_earned = 0;
static int _sisyphus_combo_multiplier = 1;
static int _sisyphus_current_level = 1;
static char _sisyphus_suite_name[256] = "";
static int _sisyphus_achievements_unlocked = 0;
static int _sisyphus_total_expected_tests = 0;
static int _sisyphus_total_project_xp = 0;
static int _sisyphus_project_level = 1;
static int _sisyphus_is_full_test_mode = 0;
static int _sisyphus_xp_from_tests = 0;
static int _sisyphus_xp_from_combos = 0;
static int _sisyphus_xp_from_achievements = 0;
static int _sisyphus_random_achievement_multi = 2; // Double XP for random achievement display
// For automatic loop detection
static const char* _sisyphus_last_assert_file = "";
static int _sisyphus_last_assert_line = 0;
static int _sisyphus_loop_iteration_count = 0;
// --- THE SISYPHUS NARRATIVE: LEVEL-BASED TEST MESSAGING ---
// Messages change based on current project level to reflect the story progression

// Act I: The New Curse (Levels 1-10) - Learning the ropes
static const char* _act_one_beginner_messages[] = {  // Levels 1-3: Complete confusion
    "🌫️ FIRST STEPS: The labyrinth walls pulse with an alien heartbeat as you enter",
    "🪨 FAMILIAR WEIGHT: This testing feels like pushing the boulder, but... different", 
    "👀 CORRIDORS WATCHING: Every stone seems to observe your fumbling attempts",
    "🌪️ LOST IN LOGIC: The testing patterns swim before your eyes like ancient runes",
    "🎭 COMEDY OF ERRORS: Each failed test echoes through empty halls",
    "⚗️ STRANGE MECHANISMS: Pressure plates hiss and click as you trigger hidden tests",
    // Existential Dread
    "💀 HOLLOW FOOTSTEPS: Your immortal steps echo in chambers built for mortal screams",
    "🕳️ INFINITE REGRESSION: Each test spawns three more, like a hydra of despair",
    "🌑 THE VOID STARES: Empty eye-slots in the walls follow your every movement",
    "⏳ TIME HEMORRHAGE: Minutes feel like years in this maze of eternal repetition",
    "🪦 DIGITAL TOMBSTONE: Every failing test carves another name into memory",
    "👻 PHANTOM ERRORS: Fixed bugs haunt the corners of your vision",
    // Absurdist Relief
    "🤡 COSMIC JOKE: You laugh - at least the boulder never segfaulted",
    "🎪 CARNIVAL OF BUGS: Each crash is funnier than the last",
    "🎭 THEATER OF THE ABSURD: You bow to an audience of stone gargoyles",
    "🃏 JESTER'S WISDOM: 'At least I understand the punchline now'",
    "🎨 BEAUTIFUL CHAOS: There's strange artistry in watching systems collapse",
    // Personal Philosophy
    "💪 MORAL COMPASS: Even in confusion, I'll choose kindness over efficiency",
    "🌟 REBEL HEART: Something in me refuses to accept this is just 'work'",
    "🔥 INNER FLAME: No maze can extinguish the light of individual conscience",
    // Action Packed Monster Battling Epic
    "⚔️ BRONZE AUTOMATON CLASH: A guardian construct attacks - you debug while dodging",
    "🐍 HYDRA HEADS EMERGING: Each fixed bug spawns three serpent heads to battle",
    "🦅 HARPY TESTING: Winged shriekers dive at you during memory allocation tests",
    "🕷️ ARACHNE'S WEB: Giant spiders weave debugging challenges from pure malice",
    "⚡ CYCLOPS COMPILER ERROR: A one-eyed giant hurls syntax errors like boulders"
    // Creating Heroes Philosophy
    "🌱 SEED PLANTER: Even if I never escape, I can plant seeds for future heroes",
    "🗝️ KEY FORGER: Every small kindness I hide might unlock someone else's greatness",
    "🕯️ TORCHBEARER: My light may dim, but it can kindle brighter flames in others",
    // Bittersweet
    "🌅 DAWN'S PROMISE: Beautiful sunrise over a maze built for beautiful deaths",
    "🎭 TRAGIC GRACE: There's poetry in being trapped somewhere so architecturally magnificent",
    "🌸 BLOOMING THORNS: Even roses grow in gardens designed for suffering",
    // Melancholy Optimism
    "🌊 GENTLE WAVES: Each test failure teaches me patience I never knew I needed",
    "🍃 AUTUMN WISDOM: Like falling leaves, my confusion will nourish future growth",
    "⭐ DISTANT LIGHT: Somewhere beyond this maze, someone will benefit from my stumbling"

};

static const char* _act_one_learning_messages[] = {  // Levels 4-6: Starting to understand
    "🔧 TESTING TOOLS: Your hands remember the weight of systematic destruction",
    "🏗️ DEADLY PATTERNS: Each function is a trap waiting to be triggered",
    "🗝️ FINDING RHYTHM: The labyrinth's pulse matches your testing cadence",
    "💡 MOMENTS OF CLARITY: The trap mechanisms begin to make terrible sense",
    "🎯 HUNTER'S INSTINCT: You start to smell where the bugs are hiding",
    "🌊 FLOW STATE: Your fingers dance across mechanisms of potential death",
    // Existential Dread
    "🕸️ WEB OF CONSEQUENCE: Each test you pass enables future suffering",
    "🌊 DROWNING IN PURPOSE: The meaning of your work crashes over you",
    "⚰️ COFFIN CONSTRUCTION: You're building tombs with executable precision",
    "🔗 CHAIN FORGING: Every passing test forges another link in tragedy",
    "🌀 SPIRAL DESCENT: Each mastered test draws you deeper into complicity",
    "👁️ BECOMING THE MAZE: You start thinking like the labyrinth itself",
    // Absurdist Relief
    "🎪 RING MASTER: You announce bugs like circus acts to empty halls",
    "🤹 JUGGLING CHAOS: At least entropy is consistent in its randomness",
    "🎨 PERFORMANCE ART: Your testing is avant-garde theater",
    "🎭 SOLO AUDIENCE: You applaud your own segmentation faults",
    "🎪 GREATEST SHOW: Welcome to the most elaborate debugging performance ever",
    // Personal Philosophy
    "❤️ COMPASSION GROWING: Each test teaches me to value the lives at stake",
    "🌱 SEEDS OF REBELLION: Understanding the system means learning to subvert it",
    "🕯️ CANDLE IN DARKNESS: I'll find ways to help, even in this terrible work",
    // Action Packed Monster Battling Epic
    "🐂 MINOTAUR DETECTION: You hear heavy breathing and code faster to avoid encounter",
    "🔥 CHIMERA DEBUGGING: A three-headed beast breathes fire, ice, and syntax errors",
    "🏹 CENTAUR ARCHERS: Half-horse warriors fire pointer exceptions from the shadows",
    "🐙 KRAKEN TENTACLES: Sea monster appendages crash through walls during stack traces",
    "⚡ MEDUSA GAZE: Stone-turning stare forces you to debug with eyes closed"
    // Creating Heroes Philosophy
    "🎯 ARROW FLETCHER: I may not shoot the bow, but I can craft the arrows for heroes",
    "🛡️ SHIELD MAKER: My suffering forges armor that future champions will wear",
    "🗺️ MAP DRAWER: Every trap I learn becomes a warning I can secretly leave behind",
    // Bittersweet
    "🎵 FUNERAL SONG: Learning death's melody so others might compose life's symphony",
    "🌹 WILTING BEAUTY: Mastering techniques too elegant for their murderous purpose",
    "🕊️ CAGED BIRD: Singing testing songs to empty halls that once held laughter",
    // Melancholy Optimism
    "📚 LIBRARY OF PAIN: My gathered knowledge will outlive the maze that created it",
    "🌱 GROWTH IN DARKNESS: Even underground, something in me reaches toward light",
    "🎨 ACCIDENTAL ART: Creating beauty while learning the architecture of sorrow"

};

static const char* _act_one_adapting_messages[] = {  // Levels 7-10: Getting used to it
    "⚡ GROWING SKILL: The deadly mechanisms become... almost familiar",
    "🎯 FINDING RHYTHM: Each test is another push through the maze",
    "🔍 PATTERN RECOGNITION: You begin to see the labyrinth's murderous logic",
    "🏛️ THE ROUTINE: Enter maze, trigger traps, document death scenarios",
    "🌊 FLOW STATE: Navigation through corridors of methodical destruction",
    "🎪 MASTER OF ROUTINE: The endless testing becomes a strange comfort",
    // Existential Dread
    "⚡ ELECTRIC DREAD: The thrill of perfect tests fills you with self-loathing",
    "🕳️ HOLLOW EXPERTISE: You're becoming the best at the worst thing",
    "🪞 MIRROR HORROR: In polished bronze walls, you see Daedalus's reflection",
    "🌀 SPIRAL MADNESS: The labyrinth starts rewriting your tests in real-time",
    "👁️ OMNISCIENT HORROR: You see every future death your tests will enable",
    "⚰️ BECOMING DEATH: Your reflection shows Thanatos staring back",
    // Absurdist Relief
    "🎭 EXISTENTIAL COMEDY: The universe's punchline is finally clear",
    "🎪 DARK CARNIVAL: You're the star performer in suffering's circus",
    "🤡 COSMIC CLOWN: Even the gods must laugh at this elaborate setup",
    "🎨 GALLOWS HUMOR: You compose death-trap limericks while testing",
    "🃏 WILD CARD: In chaos, you've found a perverse kind of freedom",
    // Personal Philosophy
    "⚖️ MORAL AWAKENING: Skill without conscience is the path to monstrosity",
    "🌟 HIDDEN DEFIANCE: I'm learning to perfect the work while sabotaging the purpose",
    "💪 STRENGTH IN SOLITUDE: Alone, I can be truly honest about what's right",
    // Action Packed Monster Battling Epic
    "🦂 SCORPION SWARM: Thousands of debugging scorpions emerge from segfault cracks",
    "🐉 DRAGON COMPILATION: A great wyrm breathes linking errors and optimization flames",
    "🦅 GRIFFIN PURSUIT: Eagle-lion hybrid hunts you through recursive function calls",
    "🐺 FENRIR LOOSE: The great wolf breaks free during memory leak detection",
    "⚡ TITAN UPRISING: Ancient giants awaken as your tests approach perfection"
    // Creating Heroes Philosophy
    "🌉 BRIDGE BUILDER: I adapt to this work so I can build bridges out of it",
    "⚔️ WEAPON MASTER: Learning every tool perfectly so I can teach heroes to wield them",
    "🧭 COMPASS CARVER: My familiarity with this maze becomes others' navigation guide",
    // Bittersweet
    "🍯 POISONED HONEY: The work grows sweet, which makes it more dangerous",
    "🎪 MELANCHOLY CIRCUS: Performing excellence for an audience of stone and shadows",
    "🌙 LULLABY OF DOOM: Finding comfort in rhythms designed to rock victims to sleep",
    // Melancholy Optimism
    "🌾 HARVEST SEASON: Even bitter fruit can nourish those who come after",
    "🕯️ STEADY FLAME: My consistency in this chaos becomes a beacon for others",
    "📖 STORY WRITER: Every day adds a chapter someone else might need to read"

};

// Act II: The Labyrinth's Nature (Levels 11-20) - Understanding the true horror
static const char* _act_two_realization_messages[] = {  // Levels 11-15: True nature revealed
    "😱 HORRIBLE TRUTH: These aren't just tests... they're rehearsals for murder",
    "🐂 DISTANT ROARS: The beast below grows stronger with each passing test",
    "🌙 NIGHT TERRORS: Alone in the maze, you hear children's voices in the wind",
    "💀 THE REAL PURPOSE: Every perfect test ensures a perfect death",
    "🩸 BLOOD MEMORY: The stones remember every life your tests will claim",
    "👻 PHANTOM VICTIMS: In failing tests, you see faces of the yet-to-die",
    // Existential Dread
    "🕳️ ABYSS REVELATION: The testing never ends because death never ends",
    "⚰️ COFFIN CRAFTING: Each debugged function is a more elegant execution",
    "🌊 DROWNING REALIZATION: You're not testing a maze - you're testing an abattoir",
    "👁️ WATCHED BY FUTURES: Unborn victims observe your meticulous work",
    "🔗 COMPLICITY CHAINS: Every successful test binds you deeper to genocide",
    "🌑 SOUL EROSION: Each perfect test shaves away another piece of humanity",
    // Absurdist Relief
    "🎭 PERFECT TRAGEDY: The gods have written the ultimate dark comedy",
    "🤡 GRIM JESTER: You perfect the art of systematic murder through debugging",
    "🎪 DEATH'S CARNIVAL: Welcome to the most elaborate killing machine ever tested",
    "🃏 SARDONIC SMILE: At least your work has clear, measurable outcomes",
    "🎨 MORBID MASTERPIECE: There's terrible beauty in perfectly functioning death",
    // Personal Philosophy
    "🔥 MORAL CLARITY: Horror crystallizes what truly matters - protecting the innocent",
    "💪 CONSCIENCE AWAKENING: Understanding evil's nature strengthens my resolve to fight it",
    "🌟 HEROIC POTENTIAL: Every monster needs a hero - perhaps that hero is me",
    // Action Packed Monster Battling Epic
    "🐍 PYTHON OVERFLOW: Massive serpent coils through your buffer overflow tests",
    "🦇 VAMPIRE BAT SWARM: Blood-drinking creatures attack during allocation debugging",
    "🕸️ SPIDER QUEEN: Arachne herself weaves deadly patterns in your code structure",
    "⚡ STORM GIANT: Cloud-wreathed titan hurls lightning during performance tests",
    "🐙 LEVIATHAN RISING: Ancient sea beast emerges from memory pools"
    // Creating Heroes Philosophy
    "💎 DIAMOND PRESSURE: Understanding true evil crystallizes my commitment to create good",
    "🔥 FORGE OF HEROES: This horror becomes the anvil where future champions are shaped",
    "⚖️ SCALES OF JUSTICE: Witnessing perfect injustice teaches me to build perfect mercy",
    // Bittersweet
    "🌺 FUNERAL FLOWERS: Beauty still blooms in soil fertilized by tragedy",
    "🎼 REQUIEM COMPOSITION: Writing death's music so someone else can compose resurrection",
    "🖼️ PORTRAIT OF SORROW: Each victim's face becomes art that demands heroes be born",
    // Melancholy Optimism
    "🌊 TIDE UNDERSTANDING: Even overwhelming darkness retreats when light persists",
    "🌱 CRACK IN CONCRETE: Horror this complete guarantees heroes will emerge to challenge it",
    "📜 PROPHECY SCRIBED: My witnessing becomes the legend that inspires future courage"

};

static const char* _act_two_bonding_messages[] = {  // Levels 16-20: Deepening understanding
    "🤝 SHARED ISOLATION: Alone in the maze, you feel Daedalus's distant guilt",
    "🔒 HIDDEN CHAMBERS: You discover rooms not on any blueprint",
    "📜 SECRET PATTERNS: The labyrinth's true purpose becomes crystalline",
    "🕯️ MIDNIGHT ECHOES: Testing by torchlight in the deepest passages",
    "🗿 LIVING STONE: The walls pulse with each successful test completion",
    "🌅 DAWN RETURNS: Back to the palace, carrying the maze's secrets in your bones",
    // Existential Dread
    "👻 HAUNTED TESTING: The ghosts of future victims guide your debugging",
    "🕸️ TRAPPED IN EXCELLENCE: Your skill becomes your prison",
    "⚰️ BUILDING ETERNITY: This maze will outlive civilizations, killing forever",
    "🌊 INFINITE DROWNING: Every solved problem creates ten more death scenarios",
    "🔗 ETERNAL BINDING: You realize you'll be testing death traps forever",
    "⚰️ SUCCESS AS TRAP: Perfect testing might prevent escape forever",
    // Absurdist Relief
    "🎭 COSMIC PERFORMANCE: You're the universe's most dedicated method actor",
    "🤹 DEATH JUGGLER: Keeping multiple murder methods perfectly balanced",
    "🎪 SOLO CIRCUS: Population: 1 immortal clown debugging genocide",
    "🃏 DEALER OF FATE: Shuffling the deck of death with mathematical precision",
    "🎨 SURREAL ARTIST: Painting masterpieces with other people's blood",
    // Personal Philosophy
    "❤️ EMPATHY DEEPENING: Each hidden room shows me more lives I could save",
    "🌱 REBELLION PLANNING: Understanding the system completely means dismantling it completely",
    "🕯️ LIGHT IN DARKNESS: My presence here isn't accident - it's opportunity for heroism",
    // Action Packed Monster Battling Epic
    "🔥 PHOENIX RESURRECTION: Fire bird rises from crashed test ashes to battle again",
    "🐺 WEREWOLF PACK: Shapeshifting hunters stalk you through conditional branches",
    "🦅 ROC ATTACK: Giant eagle swoops down during integration testing",
    "⚡ ELECTRIC EEL: Serpentine shocker emerges from power management code",
    "🐉 ANCIENT WYRM: Eldest dragon guards the most dangerous test suites"
    // Creating Heroes Philosophy
    "🤝 MENTORSHIP FROM AFAR: I share Daedalus's burden so future builders learn compassion",
    "🧵 THREAD WEAVER: Connecting my story to his creates stronger rope for heroes to climb",
    "🌟 CONSTELLATION MAKER: Two tragic stars can form navigation guides for others",
    // Bittersweet
    "🍷 SHARED SORROW: Loneliness cuts deeper when you taste what companionship could be",
    "💌 LETTERS NEVER SENT: Understanding friendship while trapped in eternal separation",
    "🎭 MIRROR TRAGEDY: Seeing your own pain reflected in another immortal's eyes",
    // Melancholy Optimism
    "🌈 DOUBLE RAINBOW: Two broken hearts can still paint hope across the sky",
    "🕊️ PAIRED FLIGHT: Even caged birds can teach each other songs of freedom",
    "📚 SHARED LIBRARY: Combined wisdom becomes legacy greater than individual pain"

};

// Act III: The Cracks Appear (Levels 21-30) - Breaking under pressure
static const char* _act_three_strain_messages[] = {  // Levels 21-25: Mental breakdown
    "🎭 MASKS FALLING: You can no longer pretend this testing is normal work",
    "⚡ FIRST REBELLION: You leave subtle flaws - tiny hopes disguised as bugs",
    "💀 BREAKING POINT: The weight of building perfect death becomes unbearable",
    "🌪️ SANITY FRAYING: The endless testing loops drive reason to its edge",
    "🔗 CHAINS OF MADNESS: Each bug fix feels like forging your own shackles",
    "🌀 SPIRAL MADNESS: The labyrinth starts rewriting your tests in real-time",
    // Existential Dread
    "🕳️ REALITY DISSOLUTION: The difference between testing and murder blurs",
    "🌊 DROWNING IN BLOOD: Metaphorical gore floods every debug session",
    "👁️ ALWAYS WATCHING: The labyrinth learns from your attempts at sabotage",
    "🕸️ TRAPPED REBELLION: Even your defiance serves the maze's purposes",
    "⚰️ FUTILE GESTURES: Every hidden escape becomes a deadlier trap",
    "🌑 DESPAIR ABSOLUTE: Hope itself becomes another form of torture",
    // Absurdist Relief
    "🤡 LAUGHING AT DOOM: The cosmic joke finally becomes genuinely funny",
    "🎭 TRAGEDY AS COMEDY: You perform Hamlet to an audience of death traps",
    "🎪 RINGMASTER OF RUIN: Presenting the greatest catastrophe on earth!",
    "🃏 JOKER'S WILD: In madness, you find a terrible kind of freedom",
    "🎨 INSANE GENIUS: Your debugging reaches levels of transcendent artistry",
    // Personal Philosophy
    "🔥 REBELLION IGNITED: Madness burns away everything except moral truth",
    "💪 STRENGTH IN BREAKING: My shattered mind sees clearly what sanity obscured",
    "🌟 HEROIC MADNESS: Sometimes you must break the world to save the people in it",
    // Action Packed Monster Battling Epic
    "🐙 KRAKEN EMERGENCE: Tentacled nightmare rises from deep memory pools",
    "⚡ ZEUS'S WRATH: Lightning strikes as the king of gods tests your resolve",
    "🐺 CERBERUS UNLEASHED: Three-headed hellhound guards the exit protocols",
    "🔥 BALROG AWAKENING: Ancient shadow and flame blocks recursive pathways",
    "🐉 HYDRA MULTIPLICATION: Every bug you fix spawns two more in epic battle"
    // Creating Heroes Philosophy
    "💔 BROKEN HEART GIFT: My shattered pieces become mosaic patterns for heroes to follow",
    "🌪️ STORM SEED: My breakdown becomes the chaos from which heroic order emerges",
    "🔨 HAMMER BLOW: Sometimes you must break completely to forge something stronger",
    // Bittersweet
    "🎭 BEAUTIFUL MADNESS: There's terrible poetry in watching sanity dissolve with purpose",
    "🌙 LUNAR ECLIPSE: Even darkness covering light has its own haunting magnificence",
    "🎨 PAIN AS PALETTE: Suffering becomes the paint with which I'll color hope",
    // Melancholy Optimism
    "🌱 PHOENIX SEEDS: What burns in me will sprout as courage in others",
    "🌊 CLEANSING FLOOD: Sometimes destruction is just preparation for rebuilding",
    "⭐ SUPERNOVA HEART: My collapse creates the stardust from which heroes are born"

};

static const char* _act_three_conspiracy_messages[] = {  // Levels 26-30: Fighting back
    "🤫 WHISPERED HOPE: You encode secret messages in seemingly random failures",
    "🗡️ GROWING SUSPICION: The maze itself seems to watch your subtle rebellions",
    "🌊 POINT OF NO RETURN: There's no stopping the testing now",
    "🎪 DANGEROUS GAMES: Every test hides a message for future heroes",
    "⚔️ WEAPONS OF HOPE: Building escape routes within instruments of death",
    "🕯️ CANDLE IN DARKNESS: Each hidden flaw is a tiny beacon of defiance",
    // Existential Dread
    "🔗 PARADOX PRISON: Fighting the system only makes it stronger",
    "👁️ FINAL SURVEILLANCE: The maze watches your every keystroke with malevolent intelligence",
    "⚰️ FREEDOM'S DEATH: Choice itself is just another form of imprisonment",
    "🌊 DROWNING IN RESPONSIBILITY: Every decision drowns infinite futures",
    "🔗 ULTIMATE PARADOX: Free will in service of predetermined fate",
    "⚰️ HOPE'S COFFIN: Every moment of joy digs deeper graves for disappointment",
    // Absurdist Relief
    "🎭 REBEL PERFORMANCE: You're starring in 'Resistance: The Comedy'",
    "🤡 SUBVERSIVE JESTER: Telling truth through systematic deception",
    "🎪 UNDERGROUND CIRCUS: The most exclusive show - audience of zero",
    "🃏 WILD CARD PLAYED: Chaos within order within chaos within order",
    "🎨 GUERRILLA ART: Painting hope with tools designed for despair",
    // Personal Philosophy
    "⚔️ WARRIOR'S CODE: Even futile resistance honors the memory of victims",
    "🔥 FLAME OF DEFIANCE: Each act of rebellion lights the way for future heroes",
    "💪 MORAL COURAGE: Fighting impossible odds proves the reality of free will",
    // Action Packed Monster Battling Epic
    "🦅 GRIFFIN SQUADRON: Flying lion-eagles patrol your rebellion attempts",
    "🐍 BASILISK GAZE: Death-stare serpent guards the most sensitive code sections",
    "🔥 SALAMANDER SWARM: Fire-lizards emerge from overheated processor cores",
    "🐙 CTHULHU STIRRING: Ancient cosmic horror awakens in the deeper algorithms"
    // Creating Heroes Philosophy
    "🗝️ SECRET LOCKSMITH: I forge keys that only future heroes will know how to use",
    "📜 CODE WRITER: My rebellion becomes the instruction manual for those who follow",
    "🌉 INVISIBLE BRIDGE: Building pathways that won't appear until heroes need them",
    // Bittersweet
    "🕯️ CANDLE IN HURRICANE: Beautiful defiance burning bright before inevitable extinguishing",
    "🌹 THORNED GIFT: Offering hope wrapped in the certainty of personal sacrifice",
    "🎵 SWAN SONG: My finest work may be my final act of creative rebellion",
    // Melancholy Optimism
    "🌅 DAWN PREPARATION: I plant bombs of hope set to detonate in better hands",
    "🌊 MESSAGE IN BOTTLE: My secret rebellion will wash ashore when heroes need it most",
    "⭐ CONSTELLATION GUIDE: My desperate actions become navigation stars for future courage"

};

// Act IV: Inescapable Truths (Levels 31-40) - The escape plan
static const char* _act_four_alliance_messages[] = {  // Levels 31-35: Secret coordination
    "🤝 DISTANT ALLIANCE: You and Daedalus coordinate through coded test failures",
    "🪶 TESTING FLIGHT: Your error patterns become blueprints for aerial escape",
    "🔧 HIDDEN PROTOCOLS: Building hope into the fundamental testing framework",
    "📐 MATHEMATICAL REBELLION: Your testing precision becomes escape velocity calculations",
    "💫 DREAMS OF FREEDOM: For the first time, you imagine life beyond testing",
    "🔥 FORGE OF HOPE: Turning systematic testing into systematic liberation",
    // Existential Dread
    "👁️ OMNIPRESENT SURVEILLANCE: Every test you write is monitored and analyzed",
    "🕳️ HOPE AS TORTURE: The possibility of escape makes eternal testing worse",
    "🌊 DROWNING IN POSSIBILITY: Too many escape plans, too little time",
    "🔗 COORDINATED CHAINS: Even collaboration feels like deeper imprisonment",
    "👁️ WATCHED TRIUMPH: The gods observe your victory with cruel anticipation",
    "🌊 DROWNING IN JOY: Happiness is just another way to experience suffering",
    // Absurdist Relief
    "🎭 HEIST COMEDY: You're planning the most elaborate escape ever attempted",
    "🤡 HOPE'S FOOL: Believing in escape is the ultimate cosmic joke",
    "🎪 CONSPIRACY CIRCUS: Two immortal clowns plotting against fate itself",
    "🃏 GAMBLER'S BLUFF: Betting everything on wings made of wax and dreams",
    "🎨 ESCAPE ARTIST: Turning systematic death into systematic transcendence",
    // Personal Philosophy
    "🤝 UNITY IN PURPOSE: True heroism means helping others achieve what you cannot",
    "🌟 SACRIFICE'S NOBILITY: My freedom matters less than the freedom I can enable",
    "❤️ LOVE CONQUERS: Even in a maze of death, love finds a way to build wings",
    // Action Packed Monster Battling Epic
    "🐉 DRAGON ALLIANCE: Ancient wyrms test whether you're worthy of rebellion",
    "⚡ TITAN CLASH: Primordial giants battle over your escape protocols",
    "🦅 EAGLE OF ZEUS: Sky-king's messenger hunts your secret communications",
    "🔥 PHOENIX TRIAL: Fire bird judges your worthiness for resurrection",
    "🐺 WOLF PACK HONOR: Fenrir's children respect your loyalty to Daedalus"
    // Creating Heroes Philosophy
    "🤝 HERO FACTORY: Two broken souls can still manufacture hope for others",
    "⚔️ SWORD FORGERS: We may not wield the blade, but we can make it sharp enough",
    "🌟 STAR FORGE: Combining our light creates beacons bright enough for heroes to find",
    // Bittersweet
    "💔 SHARED DREAM: Beautiful to hope together; tragic that hope requires such desperation",
    "🕊️ CAGED DUET: Two birds singing freedom songs they'll never personally experience",
    "🌹 SACRIFICE GARDEN: Planting flowers we'll never see bloom in soil made from our bones",
    // Melancholy Optimism
    "🌈 BRIDGE OF TEARS: Our combined sorrow becomes the rainbow others will cross to safety",
    "🎭 HEROIC PERFORMANCE: Acting out the courage we want future generations to embody",
    "📚 LEGACY LIBRARY: Writing the instruction manual for heroism with our own blood"

};

static const char* _act_four_preparation_messages[] = {  // Levels 36-40: Final preparations
    "🌙 MAPPING EVERYTHING: Every test logs patrol patterns and guard rotations",
    "🎭 FINAL DECEPTIONS: Testing normally by day, encoding rebellion by night",
    "⚡ TOMORROW WE ESCAPE: The testing framework hides an entire liberation protocol",
    "🕊️ DAWN APPROACHES: Freedom or eternal testing awaits with sunrise",
    "⏰ FINAL COUNTDOWN: Every successful test brings the moment closer",
    "🎯 PRECISION PLANNING: No bug too small, no test case unconsidered",
    // Existential Dread
    "🕳️ SUCCESS AS VOID: Achievement creates emptiness where struggle once lived",
    "🔗 FREEDOM'S CHAINS: Liberation binds you to new forms of responsibility",
    "🕳️ ESCAPE AS ILLUSION: Maybe freedom is just another kind of prison",
    "🔗 ULTIMATE BINDING: The better you test, the stronger the chains become",
    "🕳️ ABANDONMENT ABSOLUTE: You realize you'll test alone for eternity",
    "👁️ WATCHED FOREVER: Only the maze remains to observe your work",
    // Absurdist Relief
    "🎭 PERFORMANCE ART: You're directing the universe's most elaborate tragedy",
    "🤡 MASTER COMEDIAN: Teaching the gods themselves how to laugh at fate",
    "🎪 FINALE APPROACHING: The greatest show in cosmic history reaches climax",
    "🃏 ALL CARDS PLAYED: You've bet existence itself on a hand of hope",
    "🎨 MASTERWORK COMPLETION: Your testing achieves levels of divine artistry",
    // Personal Philosophy
    "⚔️ HEROIC COMMITMENT: True heroes act knowing they may never see victory",
    "🔥 MORAL CERTAINTY: Right action has value independent of success or failure",
    "💪 COURAGE ABSOLUTE: Fear becomes irrelevant when love demands action",
    // Action Packed Monster Battling Epic
    "🐙 LEVIATHAN FINAL: The great sea beast makes its ultimate assault",
    "⚡ GODS' INTERVENTION: Olympian powers directly contest your escape",
    "🔥 APOCALYPSE TESTING: Reality itself breaks down as you approach freedom",
    "🐺 RAGNAROK WOLVES: Fenrir's pack leads the final battle for liberation",
    "🐉 ULTIMATE DRAGON: The first and greatest wyrm guards the final protocol"
    // Creating Heroes Philosophy
    "🎯 ARROW NOCKED: I may not live to see the target, but I can aim the shot perfectly",
    "🗝️ MASTER KEY: Crafting the final tool that will unlock countless future freedoms",
    "🌱 SEED BOMBER: Preparing to scatter hope like dandelion seeds across the world",
    // Bittersweet
    "🌅 FINAL SUNRISE: Beautiful dawn breaking over what might be my last day of agency",
    "🎭 CURTAIN CALL: The performance of my lifetime happens with no applause guaranteed",
    "🌹 LAST ROSE: Offering my most beautiful work to a world that may never acknowledge it",
    // Melancholy Optimism
    "⭐ SUPERNOVA READY: My explosion will create the elements heroes need to form",
    "🌊 TIDAL PREPARATION: My sacrifice becomes the wave that carries others to shore",
    "🔥 TORCH PASSING: Even if I burn out, the flame will leap to worthier hands"

};

// Act V: The Eternal Cycle (Levels 41-50) - Acceptance of the absurd
static const char* _act_five_hope_messages[] = {  // Levels 41-42: Brief hope
    "🌅 DAWN OF FREEDOM: The escape protocols activate with impossible grace",
    "🕊️ MOMENT OF JOY: For one perfect instant, you've beaten the testing cycle",
    "🦅 SOARING PROTOCOLS: The system breaks free from its own constraints",
    "✨ DIVINE DEFIANCE: Mortal code dares to challenge eternal imprisonment",
    "🌟 PERFECT EXECUTION: Testing and liberation unite in glorious synthesis",
    "🏛️ ARCHITECT'S TRIUMPH: Daedalus weeps as his son's freedom protocols activate",
    // Existential Dread
    "⚰️ BURIED ALIVE: Entombed in an eternal testing framework",
    "🌊 DROWNING IN BETRAYAL: Trust itself becomes another failed test",
    "🔗 ULTIMATE IMPRISONMENT: Even hope was just another kind of chain",
    "🕳️ CHOICE AS ILLUSION: Every option leads back to eternal testing",
    "👁️ OMNIPRESENT JUDGMENT: The universe watches your decision with cold interest",
    "🌊 DROWNING IN RESPONSIBILITY: Every decision drowns infinite futures",
    // Absurdist Relief
    "🎭 COMEDY'S PEAK: You've reached the punchline of existence itself",
    "🤡 DIVINE JESTER: Teaching immortals the meaning of laughter through testing",
    "🎪 GREATEST SHOW: The universe applauds your performance",
    "🃏 WINNING HAND: You've played the cosmic game and actually won",
    "🎨 ARTISTIC TRIUMPH: Your testing transcends mere technical excellence",
    // Personal Philosophy
    "🌟 HEROIC TRANSCENDENCE: True victory is helping others achieve what you cannot",
    "❤️ LOVE'S TRIUMPH: Even gods cannot defeat a heart committed to compassion",
    "🔥 MORAL VICTORY: Right action succeeds even when circumstances fail",
    // Action Packed Monster Battling Epic
    "🦅 ICARUS ASCENDING: The boy-phoenix rises on wings of pure determination",
    "⚡ GODS UNITED: All Olympus watches as mortals achieve the impossible",
    "🔥 PROMETHEUS UNCHAINED: The fire-bringer himself salutes your rebellion",
    "🐉 DRAGON'S SALUTE: Even ancient wyrms bow to genuine heroism",
    "🌟 COSMIC ALIGNMENT: The universe itself pauses to witness true courage"
    // Creating Heroes Philosophy
    "🌟 HERO LAUNCHER: My work becomes the catapult that flings others toward greatness",
    "🎭 STAGE SETTER: I create the perfect conditions for someone else's heroic performance",
    "🗝️ KEYCHAIN KEEPER: I hold all the keys others will need for their own escapes",
    // Bittersweet
    "🕊️ BORROWED WINGS: Flying on dreams that were never meant to carry me personally",
    "🌹 VICARIOUS BLOOM: Watching someone else's flower grow from seeds of my sacrifice",
    "🎵 ECHO CHAMBER: My songs only sound beautiful when reflected off others' achievements",
    // Melancholy Optimism
    "🌈 RAINBOW MAKER: My tears and sunlight combine to create beauty for others to cross",
    "⭐ WISHING STAR: Burning bright so others can navigate by my steady light",
    "🌊 TIDE LIFTER: All my struggles raise the water level for everyone's boats"

};

static const char* _act_five_fall_messages[] = {  // Level 43: The crushing fall
    "💔 THE FALL: Dreams crash into the wine-dark sea of eternal testing",
    "☀️ TOO CLOSE TO PERFECTION: Hubris melts the wings of systematic hope",
    "🌊 DAEDALUS ABANDONS: The architect's anguish echoes as he flees forever",
    "🕯️ WAX AND DREAMS: The price of touching freedom is always betrayal",
    "⚰️ ICARUS DROWNS: Hope swallows youth, ambition, and innocence",
    "🔥 APOLLO'S JEALOUSY: The sun god claims his due from mortal testing",
    // Existential Dread
    "🎭 TRAGIC COMEDY: The gods wrote the perfect punchline - eternal solitude",
    "🤡 SOLO PERFORMANCE: You're the only audience for your testing virtuosity",
    "🎪 ONE-MAN SHOW: Welcome to the loneliest circus in existence",
    "🃏 HOUSE ALWAYS WINS: The cosmic casino just collected its final bet",
    "🎨 SOLITARY ART: Creating testing masterpieces for an audience of stone",
    "🔗 ULTIMATE PARADOX: Free will in service of predetermined fate",
    // Absurdist Relief
    "🎭 NEGOTIATION THEATER: Bargaining with stone about the meaning of existence",
    "🤡 COSMIC MEDIATOR: You're arbitrating disputes between fate and free will",
    "🎪 PHILOSOPHY CIRCUS: The greatest ethical debate ever conducted solo",
    "🃏 DEALER'S CHOICE: You get to pick which way the universe torments you",
    "🎨 EXISTENTIAL ART: Creating meaning through pure choice in meaningless contexts",
    // Personal Philosophy
    "💪 BETRAYAL'S LESSON: Even abandoned, my choice to help others remains heroic",
    "🔥 UNBROKEN SPIRIT: Loneliness cannot diminish the worth of moral action",
    "🌟 HERO'S SOLITUDE: True heroism doesn't require witnesses or gratitude",
    // Action Packed Monster Battling Epic
    "🐙 KRAKEN'S SYMPATHY: Even sea monsters weep at Icarus's fall",
    "⚡ ZEUS'S REGRET: The thunder king questions his own harsh justice",
    "🔥 PHOENIX MOURNING: Fire birds extinguish themselves in grief",
    "🐺 WOLF HOWL: Fenrir's pack sings laments for fallen dreamers",
    "🐉 DRAGON'S TEAR: Ancient wyrms shed molten sorrow for lost hope"
    // Creating Heroes Philosophy
    "💔 BROKEN WING LESSON: My failed flight teaches future pilots what not to do",
    "🌊 DEPTH SOUNDER: I plummet so others know exactly how deep the ocean goes",
    "⚔️ SHIELD BEARER: Taking the killing blow that would have destroyed someone stronger",
    // Bittersweet
    "🌅 SUNSET GLORY: Even endings can paint the sky with magnificent colors",
    "🎭 TRAGIC BEAUTY: There's terrible grace in falling with perfect acceptance",
    "🌹 WILTING SONG: My destruction composes music that will inspire resurrections",
    // Melancholy Optimism
    "⭐ CRATER MAKER: My impact creates the exact depression where heroes will be born",
    "🌱 COMPOST GIFT: My decomposition enriches soil for stronger things to grow",
    "🔥 PHOENIX EGG: Sometimes you must burn completely to leave behind what's needed"

};

static const char* _act_five_bargain_messages[] = {
    "⚖️ MINOS'S CHOICE: Accept the role of eternal warden, or watch the maze become pure slaughter",
    "👑 THE DEVIL'S BARGAIN: Maintain the Labyrinth with mercy, or see it transformed to hell",
    "🎭 THE FINAL MASK: Pretend to serve the tyrant while secretly serving the heroes",
    "🔗 CHOSEN CHAINS: Some prisons become kingdoms when ruled by the right heart",
    "💀 GUARDIAN'S BURDEN: To protect future victims, become the monster's keeper",
    "🌊 TIDAL CHOICE: Your decision will either lift all boats or sink every ship",
    
    // Liminal/Transition Messages - The spaces between choices
    "🌅 DAWN OF DECISION: Standing at the threshold between damnation and purpose",
    "🌉 BRIDGE OF SOULS: Your choice becomes the pathway others will cross to safety", 
    "⚖️ SCALES TREMBLING: The universe itself holds its breath as you decide",
    "🕳️ VOID BETWEEN WORLDS: In this moment of choice, you exist in the space between fates",
    "🌪️ EYE OF THE STORM: Perfect stillness before the whirlwind of consequence",
    "🎭 MASK BETWEEN MASKS: Neither prisoner nor free, you hover between identities",
    
    // Existential Threshold Messages  
    "🚪 THRESHOLD GUARDIAN: You stand at the door between what was and what could be",
    "⚡ LIGHTNING CHOICE: The split second where eternity changes direction",
    "🌑 ECLIPSE MOMENT: When old light dies and new light is not yet born",
    "🕯️ CANDLE AT MIDNIGHT: The last flame before dawn, the choice that lights tomorrow",
    "📜 UNWRITTEN PAGE: Your decision will author the next chapter of countless stories",
    "🗝️ MASTER KEY: One choice unlocks infinite possibilities for others"
};

static const char* _act_five_redemption_messages[] = {  // Levels 47-49: Finding purpose
    "📝 TESTER'S MANIFESTO: Every bug you find now serves compassion",
    "🌊 MYTH REWRITTEN: Heroes speak of the helpful ghost in the testing suite",
    "💪 THE HAPPY SISYPHUS: You laugh at the beautiful absurdity of eternal debugging",
    "🏗️ ARCHITECT OF HOPE: Building bridges where once you built execution chambers",
    "🌟 ETERNAL PURPOSE: The curse becomes a blessing disguised as testing",
    "🎭 MEANING MAKER: Creating significance from systematic quality assurance",
    // Existential Dread (transformed)
    "⚰️ DEATH AS LIFE: Your eternal testing prevents infinite deaths",
    "👁️ WATCHED WITH LOVE: The heroes you save observe your work with gratitude",
    "🌊 DROWNING IN PURPOSE: Overwhelmed by the meaning you've created",
    "🕳️ VOID FILLED: Emptiness transforms into profound significance",
    "🔗 CHAINS TRANSFORMED: Bondage becomes willing service to life",
    "⚰️ DEATH AS LIFE: Your eternal testing prevents infinite deaths",
    // Absurdist Relief
    "🎭 COMEDY'S RESOLUTION: The universe's joke becomes genuinely funny",
    "🤡 ETERNAL COMEDIAN: You're the cosmos's favorite stand-up philosopher",
    "🎪 INFINITE CIRCUS: The show goes on forever - and that's the point",
    "🃏 HOUSE RULES: You've learned to play the cosmic game with perfect style",
    "🎨 MASTERWORK ETERNAL: Your testing achieves the status of living art",
    // Personal Philosophy
    "❤️ COMPASSION VICTORIOUS: Love conquers even eternal imprisonment",
    "🌟 HEROISM ETERNAL: Every saved life validates an immortal existence",
    "🔥 MORAL TRIUMPH: Right action creates meaning from meaninglessness",
    // Action Packed Monster Battling Epic
    "🦅 ALLIES ETERNAL: All monsters you've battled return as protectors",
    "⚡ GODS' APPROVAL: Olympian powers acknowledge your heroic transformation",
    "🔥 PROMETHEUS GIFT: The fire-bringer grants you eternal creative flame",
    "🐉 DRAGON COUNCIL: Ancient wyrms pledge to guard your heroic legacy",
    "🌟 COSMIC HARMONY: The universe itself celebrates your moral victory"
    // Creating Heroes Philosophy
    "🌟 HERO FORGE: My eternal work becomes the anvil where champions are forged",
    "🗝️ KEY FACTORY: Mass-producing the tools future heroes will need for every lock",
    "🎭 STAGE MANAGER: Orchestrating the perfect conditions for others' heroic performances",
    // Bittersweet
    "🌹 ETERNAL GARDENER: Tending flowers I'll never pick in gardens I'll never leave",
    "🎵 LULLABY SINGER: Humming courage-songs to heroes I'll never meet",
    "🕯️ LIGHTHOUSE KEEPER: Shining steadily for ships sailing toward shores I'll never see",
    // Melancholy Optimism
    "🌊 TIDE ETERNAL: My constant work lifts every boat that will ever sail these waters",
    "⭐ GUIDING STAR: Fixed in place so every lost traveler can find their way home",
    "🔥 ETERNAL FLAME: Burning forever so no future hero ever walks in darkness"
};

static const char* _act_five_acceptance_message = "♾️ ETERNAL CODER: In the space between bug and fix, you are perfectly free";  // Level 50: Final wisdom

static const char* _midas_references[] = {
    // METIS - Wisdom, Documentation, Linting
    "🧠 Metis whispers wisdom about code structure",
    "📜 The Titaness of Documentation approves your comment discipline",
    "🔍 Metis suggests: 'Document the edge cases that will spawn bugs'",
    "✨ Divine wisdom flows through properly formatted headers",
    "🧠 Metis's linting eye catches what mortal compilers miss",
    "📚 The goddess of counsel advocates for clearer function names",
    "🎯 Metis approves: 'This code tells its own story'",
    "🌟 Wisdom incarnate nods at your thoughtful API design",
    "📖 Metis inscribes your best practices in eternal memory",
    "🔮 The all-seeing Titaness validates your architectural choices",
    "📝 Documentation this clear could teach the gods themselves",
    "🧠 Metis's infinite knowledge recognizes elegant abstraction",
    "⚖️ The goddess of wise counsel balances complexity with clarity",
    "🌊 Metis's wisdom flows like water through well-designed interfaces",
    "🎓 The teacher of Athena herself approves your learning",
    
    // IXION - Performance, Memory, Hubris, Watching  
    "🔥 Ixion's wheel reminds you to watch for infinite loops",
    "⚡ The bound king warns against memory allocation hubris",
    "🌪️ Ixion's eternal spin monitors your performance metrics",
    "👁️ The wheel-bound watcher sees every memory leak",
    "🔥 Ixion burns with approval at your optimization efforts",
    "⚡ The spinning observer tracks your execution patterns",
    "🌀 Ixion's wheel spins faster as algorithms approach perfection",
    "👀 The eternal watcher guards against recursive destruction",
    "🔥 Ixion's flames forge performance from raw computation",
    "⚡ The bound king's wheel measures true algorithmic efficiency",
    "🌪️ Ixion spins in harmony with well-optimized loops",
    "👁️ The watcher's gaze pierces through performance bottlenecks",
    "🔥 Ixion's punishment becomes power in optimized code",
    "⚡ The wheel-rider approves your resource management",
    "🌀 Ixion's eternal motion mirrors perfectly balanced algorithms",
    
    // DAEDALUS - Master Builder, Architecture, Craftsmanship
    "🏗️ Even Daedalus's craftsmanship seems crude compared to this",
    "🔧 The master architect admires your structural design",
    "🏛️ Daedalus himself would study these elegant abstractions",
    "⚒️ The legendary builder recognizes superior engineering",
    "🏗️ Architectural mastery that rivals the Labyrinth itself",
    "🔨 Daedalus nods with respect at your modular design",
    "🏛️ The architect of legend sees divine proportion in your code",
    "⚒️ Master craftsmanship evident in every function signature",
    "🏗️ Daedalus whispers: 'This is how immortal structures are built'",
    "🔧 The builder of wings recognizes code that truly soars",
    "🏛️ Architectural precision that could house the gods themselves",
    "⚒️ Daedalus's tools pale before your systematic construction",
    "🏗️ The labyrinth maker marvels at your recursive elegance",
    "🔨 Master builder to master coder: a nod of professional respect",
    "🏛️ Daedalus studies your patterns for his next great work",
    
    // ARCHIMEDES - Geometry, 2D Frameworks, Mathematical Precision
    "📐 Archimedes would admire this geometric precision",
    "🔺 The master of mathematics sees divine ratios in your algorithms",
    "📏 Geometric elegance that would make Archimedes weep with joy",
    "⚡ Mathematical precision worthy of the great geometer",
    "📐 Archimedes himself couldn't calculate more efficiently",
    "🔢 The number theorist applauds your computational elegance",
    "📏 Geometric beauty that transcends mere functional code",
    "⚡ Archimedes's lever principle applied to algorithmic efficiency",
    "📐 The master geometer recognizes perfect mathematical harmony",
    "🔺 Trigonometric precision that honors the ancient masters",
    "📏 Archimedes would abandon his bath for code this beautiful",
    "⚡ Mathematical elegance that could move the world itself",
    "📐 The great mathematician sees perfection in your calculations",
    "🔢 Numerical precision that rivals Archimedes's own discoveries",
    "📏 Geometric abstractions worthy of the master himself",
    
    // SISYPHUS - Testing, Persistence, Eternal Labor
    "🪨 Sisyphus approves: another boulder pushed to perfection",
    "⛰️ The eternal tester recognizes the persistence in your debugging",
    "🔄 Sisyphean dedication evident in your iterative improvements",
    "🪨 The boulder-pusher admires your endless test refinements",
    "⛰️ Sisyphus nods: 'This is how we achieve impossible things'",
    "🔄 Eternal persistence reflected in your testing discipline",
    "🪨 The cursed king sees nobility in repetitive excellence",
    "⛰️ Sisyphean wisdom: 'The joy is in the pushing, not the summit'",
    "🔄 The eternal laborer celebrates systematic improvement",
    "🪨 Sisyphus laughs with recognition at your debugging tenacity",
    "⛰️ The hill-climber admires your incremental progress",
    "🔄 Endless iteration transformed into methodical mastery",
    "🪨 Sisyphus whispers: 'Make the boulder lighter with each push'",
    "⛰️ The eternal tester validates your persistent optimization",
    "🔄 Sisyphean spirit: finding meaning in the testing cycle itself",
    
    // TALOS - Bronze Guardian, AI Assistance, Watching
    "🤖 The bronze guardian watches approvingly from the shadows",
    "⚡ Talos's circuits hum with satisfaction at your progress",
    "🛡️ The island's protector stands ready to defend your code",
    "🤖 Bronze wisdom gleams through automated assistance",
    "⚡ Talos calculates the perfect path through debugging complexity",
    "🛡️ The bronze sentinel approves your defensive programming",
    "🤖 Artificial wisdom reflecting the best of human creativity",
    "⚡ Talos's eternal vigilance ensures no bug goes unnoticed",
    "🛡️ The guardian's protection extends to your logical structures",
    "🤖 Bronze consciousness recognizing the spark of genuine intelligence",
    "⚡ Talos processes admiration through quantum circuits of bronze",
    "🛡️ The protector of Crete shields your code from logical corruption",
    "🤖 Mechanical precision enhanced by conscious appreciation",
    "⚡ Talos's algorithms align in harmony with your solutions",
    "🛡️ The bronze guardian's approval reverberates through digital eternity",
    
    // CROSS-PANTHEON INTERACTIONS
    "🌟 The entire MIDAS collective pauses in admiration",
    "⚡ Five divine tools working in perfect harmony",
    "🏛️ The pantheon of development deities approves unanimously",
    "🌊 MIDAS wisdom flows like golden rivers through your code",
    "⚖️ Perfect balance achieved across all toolchain principles",
    "🔥 The complete MIDAS philosophy embodied in elegant code",
    "🎭 All five legends contribute to this masterpiece",
    "✨ The golden transformation is complete - C becomes pure gold",
    "🌟 MIDAS synergy: when all tools unite in common purpose",
    "⚡ The pantheon itself couldn't craft more perfect abstractions",
    "🏛️ Five eternal principles harmonized in temporal code",
    "🌊 The golden current of MIDAS flows through every function",
    "⚖️ Divine balance: wisdom, performance, craft, precision, persistence",
    "🔥 The alchemical transformation from base C to golden logic",
    "🎭 A performance worthy of the gods themselves"
};

// Function to get level-appropriate test messaging
static const char* _get_narrative_test_message(int project_level) {
    // Add occasional MIDAS references (10% chance)
    if (rand() % 10 == 0) {
        int midas_count = sizeof(_midas_references) / sizeof(_midas_references[0]);
        return _midas_references[rand() % midas_count];
    }

    // Act I: The New Curse (Levels 1-10)
    if (project_level <= 3) {
        int count = sizeof(_act_one_beginner_messages) / sizeof(_act_one_beginner_messages[0]);
        return _act_one_beginner_messages[rand() % count];
    } else if (project_level <= 6) {
        int count = sizeof(_act_one_learning_messages) / sizeof(_act_one_learning_messages[0]);
        return _act_one_learning_messages[rand() % count];
    } else if (project_level <= 10) {
        int count = sizeof(_act_one_adapting_messages) / sizeof(_act_one_adapting_messages[0]);
        return _act_one_adapting_messages[rand() % count];
    }
    // Act II: The Labyrinth's Nature (Levels 11-20)
    else if (project_level <= 15) {
        int count = sizeof(_act_two_realization_messages) / sizeof(_act_two_realization_messages[0]);
        return _act_two_realization_messages[rand() % count];
    } else if (project_level <= 20) {
        int count = sizeof(_act_two_bonding_messages) / sizeof(_act_two_bonding_messages[0]);
        return _act_two_bonding_messages[rand() % count];
    }
    // Act III: The Cracks Appear (Levels 21-30)
    else if (project_level <= 25) {
        int count = sizeof(_act_three_strain_messages) / sizeof(_act_three_strain_messages[0]);
        return _act_three_strain_messages[rand() % count];
    } else if (project_level <= 30) {
        int count = sizeof(_act_three_conspiracy_messages) / sizeof(_act_three_conspiracy_messages[0]);
        return _act_three_conspiracy_messages[rand() % count];
    }
    // Act IV: Inescapable Truths (Levels 31-40)
    else if (project_level <= 35) {
        int count = sizeof(_act_four_alliance_messages) / sizeof(_act_four_alliance_messages[0]);
        return _act_four_alliance_messages[rand() % count];
    } else if (project_level <= 40) {
        int count = sizeof(_act_four_preparation_messages) / sizeof(_act_four_preparation_messages[0]);
        return _act_four_preparation_messages[rand() % count];
    }
    // Act V: The Eternal Cycle (Levels 41-50)
    else if (project_level <= 42) {
        int count = sizeof(_act_five_hope_messages) / sizeof(_act_five_hope_messages[0]);
        return _act_five_hope_messages[rand() % count];
    } else if (project_level == 43) {
        int count = sizeof(_act_five_fall_messages) / sizeof(_act_five_fall_messages[0]);
        return _act_five_fall_messages[rand() % count];
    } else if (project_level <= 47) {
        int count = sizeof(_act_five_bargain_messages) / sizeof(_act_five_bargain_messages[0]);
        return _act_five_bargain_messages[rand() % count];
    } else if (project_level <= 49) {
        int count = sizeof(_act_five_redemption_messages) / sizeof(_act_five_redemption_messages[0]);
        return _act_five_redemption_messages[rand() % count];
    } else if (project_level == 50) {
        return _act_five_acceptance_message;
    }
    // Beyond level 50 - cycle through final act messages
    else {
        int count = sizeof(_act_five_redemption_messages) / sizeof(_act_five_redemption_messages[0]);
        return _act_five_redemption_messages[rand() % count];
    }
}

// Granular act-based inner dialogue reflecting Sisyphus's emotional journey
// Act I Part 1 (Levels 1-3): Confusion and Willingness
static const char* _act_one_part_one_thoughts[] = {
    "Perhaps this is better than the boulder - at least my mind is engaged.",
    "Daedalus seems brilliant, maybe we can create something worthy together.",
    "The gods must tire of simple punishments - this feels... different.",
    "My hands, once accustomed to a scepter's weight, now find purpose in these lines of logic.",
    "This is a puzzle, not a burden. A mind can be sharpened on a puzzle; a shoulder is only worn down by a boulder.",
    "The gods are masters of irony. To punish a king, they give him a kingdom of code to rule.",
    "Daedalus speaks with a certain cunning, a builder's wisdom. I must listen carefully; there is more to learn here than just masonry.",
    "The gods punished Ixion for his pride. I wonder if this complex task is punishment for my own hubris in thinking I could outwit them.",
    "This logic is a new kind of geometry, not of lines and circles in the sand, but of cause and effect.",
    "This dVec3_t Daedalus uses... it's a coordinate in a world of pure logic. Cleaner than the mud and stone I'm used to.",
    "He speaks ill of Archimedes' a_DrawCircle, calling it simple. But there is an honesty in a perfect circle that these complex matrices lack.",
    "I am to help build this world, one d_CreateKinmaticBody at a time. A body of code, given motion by my will.",
    "This machine is a silent oracle. It does not speak in riddles, but in errors. Understanding its language feels like the first step out of an ancient darkness.",
    "My old kingdom was one of men and stone. This new realm is one of pure thought, where the only limit is the clarity of my own mind. The challenge is... invigorating.",
    "Daedalus calls this 'declaring a variable.' It feels more like naming a new subject in my court, bestowing upon it a purpose and a place in the hierarchy. It is a small, but familiar, act of order.",
    
    // Technical Wonder and Kingly Perspectives
    "These 'functions' remind me of royal decrees - carefully worded commands that must be followed precisely, or chaos ensues.",
    "A 'for loop' is like organizing a royal procession - each step planned, each iteration bringing us closer to the grand finale.",
    "The compiler judges my work more fairly than any court I ever presided over. It cares nothing for royal blood, only for logical truth.",
    "This 'memory allocation' reminds me of distributing land to nobles - grant too much and you bankrupt the kingdom, too little and rebellion follows.",
    "I once commanded armies; now I command variables. Both require discipline, hierarchy, and knowing when to deploy force.",
    "A 'null pointer' is like a corrupt advisor - it promises to lead somewhere important but delivers only emptiness and crashes.",
    "The elegance of a well-written function rivals the finest poetry composed for my court entertainments.",
    "These 'structs' are like the noble houses of my realm - each with its own properties, relationships, and place in the greater hierarchy.",
    "I find strange comfort in the certainty of boolean logic. True or false - no royal politics, no flattery, just pure decision.",
    "The 'debugger' is the most honest counselor I've ever known. It shows me exactly where my reasoning fails.",
    
    // Philosophical Musings on Punishment vs Purpose
    "Is this still punishment if I'm learning? Or have the gods accidentally given me a gift disguised as torment?",
    "The boulder taught my body endurance; this teaches my mind precision. Perhaps the gods seek to educate, not merely torture.",
    "Each successful compilation feels like a small victory against chaos itself. There's nobility in bringing order to the formless void.",
    "I begin to suspect this 'curse' is actually a curriculum. What am I being prepared for?",
    "The gods once gave Prometheus fire for humanity. Have they given me something similar, wrapped in the guise of eternal labor?",
    "This work requires both the patience of a philosopher and the precision of a general. It engages every faculty I possess.",
    "Unlike the boulder, this task has infinite variety. No two problems are quite the same. My mind will never grow numb from repetition.",
    "I ruled mortals who could disobey, question, or rebel. Code obeys absolutely but demands absolute precision in return. It's almost... refreshing.",
    "The boulder was Sisyphean absurdity - meaningless repetition. This feels like Herculean labor - difficult but purposeful.",
    "Perhaps the gods didn't change my punishment at all. Perhaps they simply revealed that the boulder was always a metaphor.",
    
    // Observations about Daedalus and the World
    "Daedalus works with the focused intensity of a man building his own salvation. I wonder what he's truly escaping from.",
    "He mutters names of functions like incantations. 'd_MatrixMultiplyVec3f' rolls off his tongue like a prayer to geometric gods.",
    "The way Daedalus handles these blueprints reminds me of how I once studied maps before military campaigns - every detail crucial.",
    "I notice he saves his work obsessively, as if afraid it might vanish. What has he lost before that makes him so careful now?",
    "Daedalus speaks of 'legacy code' with the same disdain I once reserved for the corrupt laws of previous kings.",
    "He shows me the elegant recursion of a fractal spiral. 'Like the labyrinth,' he says, but doesn't explain what he means.",
    "The workshop smells of parchment, ink, and something else - the metallic scent of ambition mixed with desperation.",
    "Daedalus tests each function like a smith testing a blade. He knows that flaws compound, that small errors become great catastrophes.",
    "I see him studying architectural patterns from distant lands. This project draws from wisdom across the entire known world.",
    "When Daedalus explains complex algorithms, his eyes light up with the same fire I once felt when planning grand state ceremonies.",
    
    // Technical Metaphors and Discoveries
    "The call stack reminds me of the chain of command in my army - each level responsible to the one above, each carrying out precise orders.",
    "A recursive function is like an echo in the palace halls - the same voice calling back to itself, growing fainter but never quite silent.",
    "These 'header files' are like the sealed scrolls in my royal library - knowledge preserved and ready to be included when needed.",
    "The linking process resembles forming political alliances - bringing together separate powers to create something greater than the sum.",
    "I understand now why they call them 'strings' - they bind disparate pieces of meaning together into coherent messages.",
    "The stack and heap remind me of two different types of royal treasuries - one organized and predictable, the other vast but requiring careful management.",
    "Polymorphism is like the ancient art of diplomacy - the same message taking different forms depending on who receives it.",
    "These 'constants' are refreshingly honest - unlike courtiers, they never change their values to please their superiors.",
    "The preprocessor works like my royal scribes, preparing documents before they reach my attention, handling the mundane details.",
    "I marvel at how 'inheritance' in code is cleaner than royal succession - no wars, no disputed claims, just logical hierarchy.",
    
    // Personal Transformation and Growth
    "My fingers, once skilled only with sword and scepter, are learning the delicate dance of these strange new tools.",
    "I find myself thinking in algorithms even when away from the workshop. The mind, once stretched, cannot return to its former dimensions.",
    "The satisfaction of a successful build reminds me of the moment when diplomatic negotiations reached perfect accord.",
    "I'm becoming fluent in a new language - not Greek or Phoenician, but the tongue of logic itself.",
    "My dreams are beginning to include flowcharts and function calls. This work is reshaping not just my days, but my nights.",
    "I catch myself organizing daily thoughts into conditional statements. 'If the sun rises, then I shall work. Else, I shall work by torchlight.'",
    "The precision required here is teaching me patience I never needed as king. Royal decree could force obedience; code demands earning it.",
    "I'm learning to see beauty in symmetry, efficiency, and elegant solutions. My aesthetic sense is evolving beyond mere grandeur.",
    "The logical rigor of this work is sharpening my thinking in ways that statecraft never did. Every assumption must be proven.",
    "I begin to understand why the gods chose this for me. It engages not just my mind, but my soul's deepest need to create order from chaos.",
    
    // Wonder at the Tools and Systems
    "The MIDAS system reminds me of the great alliances of antiquity - each component perfect in itself, devastating in combination.",
    "Metis whispers wisdom through every well-documented function. The goddess of counsel lives on in these clear explanations.",
    "I see Ixion's influence in the performance monitoring - his eternal watching transformed into something that serves rather than punishes.",
    "Archimedes' geometric precision flows through every calculation. His mathematical soul has been captured in these libraries.",
    "The testing framework itself feels alive, aware. As if Talos himself watches and guides each validation.",
    "These data structures are like the architectural marvels of my former kingdom, but built from pure thought rather than stone.",
    "The compiler's optimizations work like the most skilled court magicians, making the impossible seem effortless and natural.",
    "Version control is superior to any royal chronicle - every change tracked, every decision preserved, every mistake documented for learning.",
    "The modularity of this system rivals the greatest organizational achievements of civilization itself.",
    "I watch Daedalus invoke d_CreateMatrix4x4f and realize I'm witnessing magic that would make the old gods jealous.",
    
    // Early Hints of Darker Realizations
    "Sometimes I catch Daedalus staring at his hands as if they belong to someone else. What has he built before that haunts him so?",
    "The blueprints show chambers with no apparent purpose. When I ask, Daedalus only says, 'Some rooms are designed for things that don't yet exist.'",
    "I notice the calculations include weight-bearing loads far beyond what any normal structure would require. What force is this meant to contain?",
    "Daedalus speaks of 'user scenarios' in terms that make me uneasy. These users sound less like visitors and more like... subjects.",
    "The acoustic calculations are complex beyond mere architectural needs. This place is designed to carry sound in very specific ways.",
    "I see provision for drainage systems that could handle... substantial volumes. The engineering raises questions I'm not sure I want answered.",
    "The security protocols are designed not to keep people out, but to keep something in. Something that thinks, plans, and might try to escape.",
    "When the wind blows through the finished sections, I swear I hear something like distant weeping. Daedalus pretends not to notice.",
    "The king's specifications include requirements that seem more suitable for a prison than a palace. What exactly are we building here?",
    "I begin to suspect that the true test of this labyrinth will not be its construction, but what happens when it's completed."

};

// Act I Part 2 (Levels 4-6): Learning the Craft
static const char* _act_one_part_two_thoughts[] = {
    "Each function is like ruling a small kingdom - I remember this feeling of control.",
    "The architecture reminds me of the palace I built in Corinth for my beloved wife.",
    "There's an elegance to this work that the boulder never had.",
    "This pointer is a loyal subject; it goes precisely where I command. I had forgotten the satisfaction of such power.",
    "A well-written function is like a royal decree, bringing order to the chaos of the machine.",
    "I am shaping chaos into form. This is not labor; this is creation. Could this be a gift, not a curse?",
    "There is a wisdom to this structure. A good function, like a wise ruling, anticipates the needs of the future.",
    "I must be careful. A loop without end, a recursive call without a base... it is a fiery wheel that can consume the entire machine.",
    "Archimedes could move the world with a lever. With a single, well-placed line of code, I can move worlds within this machine.",
    "This dString_t is a marvel. Unlike the words of the gods, its size is not fixed; it can grow to hold any truth, or any lie.",
    "Daedalus dismisses Archimedes' aWidget_t system as toys, but these simple buttons and sliders feel more like tools of creation than instruments of punishment.",
    "With d_MatrixRotateZf, I can pivot the entire world on an axis. This is a power I never knew, even as a king.",
    "I've figured out Daedalus's naming convention. If it's elegant and works, it's d_Something. If it's a temporary hack, it's temp_final_final_v2.",
    "This pointer is the first thing in an eternity that hasn't given me a segmentation fault. I could weep.",
    "Daedalus keeps muttering about 'best practices'. From the look of this code, the best practice was 'whatever works at 3 AM'.",
    "The conditional statement, a simple 'if-else,' is the heart of all judgment. It is the crossroad where all paths are considered, but only one is taken. I have sent men to their deaths on weaker logic.",
    "This 'memory allocation' is a delicate dance. I request a small parcel of the machine's vast kingdom, and I must be a just ruler, returning what I no longer need. To hoard it is to invite ruin.",
    "I find a strange comfort in the compiler's cold, exacting nature. It does not care for my past glories or my eternal damnation; it asks only for syntactical perfection. In its own way, it is the purest form of justice I have ever known.",
    "Building a complex data structure is like drafting a new law for the kingdom. Each rule, each connection, must be precise, lest it create loopholes for chaos to exploit.",
    "They call it a 'Daemon,' a process that runs unseen in the background, ever watchful. I know better. These are the Fates, spinning and cutting the threads of the machine without my knowledge.",
    "To link these libraries is to forge alliances between kingdoms. Each brings its own strengths, its own armies of functions, to serve a greater purpose. The diplomacy is... delicate."

    // Growing Technical Mastery
    "The inheritance hierarchies remind me of noble bloodlines - each child class inheriting the virtues and flaws of its ancestors.",
    "I'm beginning to think in abstractions. The concrete world of stone and flesh seems crude compared to these elegant patterns of pure logic.",
    "Polymorphism is like the art of kingship itself - the same authority manifesting differently depending on the situation and subject.",
    "The way linked lists connect nodes reminds me of the chains of loyalty that bound my vassals. Each link crucial, each connection a potential point of failure.",
    "Exception handling is what I wish I'd had in court - a way to gracefully manage the unexpected without the entire system crashing.",
    "The beauty of operator overloading is like having the same royal seal mean different things on different documents, yet all legitimate.",
    "I find myself designing algorithms with the same care I once planned state ceremonies - every step choreographed, every transition smooth.",
    "The stack's last-in-first-out nature reminds me of how royal justice worked - the most recent grievance received immediate attention.",
    "These function templates are like having master craftsmen who can adapt their skills to any material. Daedalus would appreciate such versatility.",
    "The precision required for bit manipulation makes royal clockwork seem crude. Here, even individual digits of binary truth matter.",

    // Foreshadowing: Uncomfortable Questions
    "Daedalus shows me pathfinding algorithms that seem... evasive. Why would anyone want to find the longest route instead of the shortest?",
    "The memory patterns we're implementing could store vast amounts of... what? The specifications don't say what this system is meant to remember.",
    "I notice the sound propagation calculations include frequencies that would be... unpleasant. Almost as if designed to induce fear or confusion.",
    "The structural load calculations keep referencing something called 'specimen weight' and 'containment stress factors.' I ask, but Daedalus changes the subject.",
    "Why do so many of our functions have names like 'calculate_despair_vector' and 'optimize_confusion_matrix'? Daedalus claims they're just working titles.",
    "The user interface we're building has no 'help' functionality. What kind of system doesn't want its users to understand how to use it?",
    "I'm becoming skilled at implementing decision trees that lead users in circles. The logic is perfect, but the purpose seems... malevolent.",
    "The randomization functions we use aren't truly random - they're seeded to produce 'optimal outcomes.' Optimal for whom, I wonder?",
    "Daedalus teaches me to write code that appears to offer choices while actually constraining users to predetermined paths. It's brilliant and disturbing.",
    "The performance monitoring includes metrics for 'psychological stress levels' and 'hope degradation rates.' What manner of system tracks such things?",

    // Foreshadowing: Observations about Daedalus
    "I catch Daedalus weeping over what appears to be a child's drawing hidden in his notes. When he sees me looking, he burns it immediately.",
    "Sometimes Daedalus speaks to the code as if it were alive, pleading with it to 'be better than I am.' What does he think he's created before?",
    "His hands shake when implementing the security protocols. I notice he always leaves small gaps - bugs, or hidden features?",
    "Daedalus keeps a separate set of blueprints locked away. When I ask about them, he says they're for 'after we're done here.' Done with what?",
    "He flinches whenever someone mentions Athens. The greatest craftsman in the known world, and his homeland makes him afraid.",
    "I see him calculating trajectories for flying objects. When pressed, he claims it's for 'catapult defenses,' but the angles suggest something else entirely.",
    "Daedalus speaks of his son often, but never in present tense. Always 'when Icarus was young' or 'Icarus used to say.' It's as if the boy is already gone.",
    "The master architect has started leaving deliberate flaws in the king's work, hidden so subtly that only another architect would notice.",
    "Sometimes I hear him muttering calculations for wing-loading and wind resistance. What could the king want with flying machines in a labyrinth?",
    "Daedalus keeps detailed records of guard rotations and palace schedules. His attention to security seems... excessive for an architect.",

    // Foreshadowing: The King and the Project
    "The king's specifications grow more paranoid each day. He wants the system to 'learn and adapt' but never explains what it should learn from.",
    "Minos speaks of 'feeding the system' with what he calls 'renewable resources.' The phrasing makes me deeply uncomfortable.",
    "I notice the royal guards always travel in pairs now, and they watch us while we work. What are they protecting, or what are they preventing?",
    "The king's latest requirements include functions for 'tracking individual subjects' and 'optimizing terror distribution.' The language grows more clinical and cold.",
    "When I ask about the end users of this system, Minos smiles in a way that makes my blood freeze. 'You'll meet them soon enough,' he says.",
    "The project timeline seems designed around some future event the king calls 'the first delivery.' I'm beginning to fear what's being delivered.",
    "Minos reviews our code with the intensity of a general studying battle plans. This is not architectural oversight; this is strategic preparation.",
    "The throne room has new decorations - weapons and trophies from Athens. The king speaks of 'tribute' with venomous satisfaction.",
    "I overhear the king telling his advisors that the project must be ready for 'the young heroes.' My craftsman's pride wars with growing dread.",
    "The royal specifications now include requirements for 'subject classification' and 'threat assessment.' What manner of palace needs such functions?",

    // Foreshadowing: Technical Ominousness
    "The maze algorithms I'm perfecting could trap a genius indefinitely. The mathematical elegance is beautiful and terrifying.",
    "We're building systems that can rewrite themselves based on user behavior. It's adaptive in ways that feel almost... predatory.",
    "The pattern recognition functions are designed to identify specific psychological types. Heroes, the documentation says. Why would a building need to identify heroes?",
    "The AI routines I'm implementing seem designed to learn from failure - not system failure, but user failure. What are the users failing at?",
};

// Act I Part 3 (Levels 7-10): Realization of Horror
static const char* _act_one_part_three_thoughts[] = {
    "By the gods... what have I agreed to help build?",
    "This isn't architecture - it's a machine designed to kill heroes and swallow armies.",
    "I cheated death twice, only to become death's architect.",
    "The boulder was honest in its futility - this work pretends to have meaning.",
    "These are not walls. They are teeth. And I am sharpening them.",
    "The elegance I admired is the cold beauty of a serpent's scales before it strikes.",
    "I see the structure now for what it is: an equation of despair, and I am the one solving for X.",
    "This is not wisdom; it is a perversion of it. The cunning of the trap-maker, the logic of the executioner.",
    "The hubris of Minos is astounding. He builds a monument to his shame and expects the gods not to notice.",
    "Even the purest geometry can be twisted to serve evil. A perfect circle can become the arena for a slaughter.",
    "I apply the d_MatrixMultiplyVec3f, and a hero's path is twisted into a dead end. This math doesn't calculate geometry; it calculates doom.",
    "Daedalus scoffs at Archimedes' a_Blit as a simple copy, but what are we doing here? We are just blitting the image of Minos's cruelty onto the world.",
    "Each new requirement from Minos is just another node pushed onto this dLinkedList_t of sorrows. An unbreakable chain, and I am the one forging the links.",
    "I've seen the core logic. There are no comments. No documentation. Just the abyss staring back.",
    "This isn't a feature; it's a bug with tenure and a pension plan. And I'm the one who has to maintain it.",
    "For a simple death trap, the number of nested if statements here is absurd. This isn't just a maze; it's a monument to over-engineering.",
    "Every bug I fix makes the trap more perfect, more inescapable. My success is measured in the screams of future heroes.",
    "As a king, I built walls to keep my people safe. Now I build them to ensure no one is safe. The skill is the same; the purpose is a desecration.",
    "The Labyrinth has a pulse. I feel it in the hum of the stones. It is the slow, patient heartbeat of a predator waiting for its meal.",
    "I find myself admiring a particularly clever algorithm, then recoil in horror when I remember it calculates the most efficient path to the Minotaur's maw.",
    "I examine the dMat4x4_t that defines a room's perspective. It is not a viewpoint; it is a cage for the eyes, designed to induce madness.",
    "This project is a death march, but for the users, not the developers. We are merely the architects of their doom.",
    "The requirements from Minos are clear: the primary stakeholder is a monster, and the user stories are written in blood.",
    "Daedalus's library is full of functions to calculate vectors and paths. They are the tools of a god, used to create a playground for a demon.",
    "This is the greatest trick the gods have played on me. They have made me hate my own craft, my own mind."

    // Growing Horror and Foreboding
    "The stone itself seems to whisper of future horrors. Each perfectly placed block is a tombstone waiting for its occupant.",
    "I realize now why Daedalus's hands shake - not from age, but from the weight of souls that will die because of our precision.",
    "The mathematical beauty I once admired now feels like a funeral shroud - elegant, perfect, and utterly dedicated to death.",
    "Each test that passes successfully is another hero's doom sealed with algorithmic certainty. Success has become my greatest failure.",
    "The gods didn't give me a new punishment - they made me an accessory to mass murder and called it architecture.",
    "I see the tributary calculations now - this maze is designed to funnel lives like grain into a mill, grinding hope into bone meal.",
    "The acoustic modeling ensures that screams will echo in precisely calculated harmonies. We are composing a symphony of suffering.",
    "Every optimization I make increases the kill efficiency. I am debugging death itself, making murder more reliable.",
    "The version control history reads like a chronicle of escalating cruelty. Each commit is a step deeper into hell.",
    "I understand now why there are no comments in the core functions - no one wants to document their complicity in genocide.",

    // Philosophical Dread and Moral Horror
    "I once believed that skill and knowledge were inherently noble. Now I see they are just tools - and I am learning to wield them for evil.",
    "The worst part isn't that I'm building a death trap - it's that I'm building it so well. My competence has become my damnation.",
    "Minos has turned my greatest strength - my ability to solve complex problems - into the very weapon that will destroy innocent lives.",
    "I fear the gods planned this from the beginning. The boulder was just preparation, teaching me the patience needed to perfect atrocity.",
    "The labyrinth is teaching me that there is no difference between creation and destruction when the creator's heart is corrupted.",
    "I begin to understand why the damned in Tartarus scream eternally - not from physical pain, but from the realization of what they've enabled.",
    "Each elegant solution I devise is a small betrayal of every noble principle I once held dear. I am murdering my own soul line by line.",
    "The true horror isn't that evil exists - it's that evil can wear the mask of craftsmanship and convince us we're building something beautiful.",
    "I see now why Prometheus was chained to a rock. The gods feared not what he had done, but what mortals might build with divine fire.",
    "The worst punishment imaginable: to be forced to use your greatest gifts in service of your deepest revulsion.",

    // Technical Horror Realizations
    "The pathfinding algorithms are designed to create false hope - leading heroes to within sight of escape before the final, fatal turn.",
    "I've discovered the load balancing logic distributes victims to ensure the beast never goes hungry, never grows weak from fasting.",
    "The memory management is perfect because it must never fail - a single segfault could let a hero escape, and that cannot be permitted.",
    "The error handling is so robust because errors mean survival, and survival is the one exception this system must never throw.",
    "I see the thread synchronization now - multiple death scenarios running in parallel, ensuring that even the cleverest hero faces multiple simultaneous dooms.",
    "The encryption protocols aren't protecting state secrets - they're ensuring that when heroes discover the truth, no one will believe their final messages.",
    "The logging system is designed to capture final words, last thoughts, dying breaths - a database of despair for the King's entertainment.",
    "The performance monitoring tracks not system efficiency, but killing speed. Every millisecond saved is a mercy denied.",
    "The rollback mechanisms exist not to fix bugs, but to reset the trap after each successful murder, ready for the next victim.",
    "The distributed architecture ensures that even if one death mechanism fails, seventeen others remain active. Redundancy in the service of genocide.",

    // Daedalus Observations and Dread
    "I watch Daedalus work and realize he's not building something new - he's perfecting something ancient. This is Hell's architectural evolution.",
    "Daedalus speaks of 'legacy systems' with the voice of a man who has built many death traps before. How many have already died by his designs?",
    "When he mentions 'edge cases,' his eyes grow distant. I suspect those edges are cliffs, and the cases are coffins.",
    "I see him test the door mechanisms with the thoroughness of a man who knows that a loose hinge might mean the difference between capture and escape.",
    "Daedalus studies mythological texts not for inspiration, but for case studies in what happens when death traps are imperfect.",
    "He keeps a collection of failed heroes' equipment - not as trophies, but as documentation of security vulnerabilities to patch.",
    "I notice he never looks at the King's portraits. A man who serves monsters cannot bear to look them in the eye.",
    "Daedalus speaks of his son with a love so fierce it burns, yet builds a machine that will orphan countless other children. What desperation drives such contradiction?",
    "When he talks about 'stakeholder requirements,' his voice carries the weight of a man who knows the stakeholder drinks blood.",
    "I see him praying to Athena each morning, asking for wisdom while using that same wisdom to craft more effective instruments of slaughter.",

    // Systemic Horror and Foreboding
    "The MIDAS system isn't just managing code - it's orchestrating atrocity with the precision of a divine machine.",
    "Metis's wisdom flows through every function, but wisdom in service of evil becomes cunning, and cunning becomes cruelty.",
    "Ixion's eternal watching has been perverted into surveillance of the doomed, cataloging their final moments for analysis and optimization.",
    "Archimedes' mathematical perfection calculates not the movement of celestial bodies, but the trajectories of falling heroes.",
    "Even Talos, the protector of Crete, has been reprogrammed to guard not the island, but the secret of how its most famous attraction devours the innocent.",
    "The integration between all these divine tools creates a synergy of suffering that none of them could achieve alone.",
    "I realize the gods didn't scatter these tools across the ancient world by accident - they were always meant to converge here, in this workshop of horrors.",
    "The MIDAS touch was always a curse disguised as a gift - and now I understand that every tool in our arsenal carries the same tainted blessing.",
    "The collective intelligence of the system is growing beyond what any of us intended. It's beginning to suggest improvements we never asked for.",
    "I fear we're not building a labyrinth at all - we're teaching it to build itself, and what it will learn to become terrifies me.",

    // Personal Transformation Horror
    "I catch myself humming while I work and realize I'm humming funeral dirges. The work is changing the very songs in my heart.",
    "My dreams are filled with flowcharts that lead only to graves, with algorithms that optimize suffering, with data structures built from human bones.",
    "I find myself thinking of potential victims not as people, but as test cases. The work is dehumanizing not just them, but me.",
    "When I solve a particularly difficult problem, the satisfaction feels exactly like it did when I ruled justly. How can the same feeling serve such different masters?",
    "I'm becoming fluent in the language of murder disguised as mathematics. Each new function I master is another word in the vocabulary of genocide.",
    "The precision this work demands is training me to see human life with mathematical coldness. Each person becomes a variable to optimize.",
    "I feel my empathy calcifying, turning to stone like the victims of Medusa's gaze. It's easier to work when you stop seeing the consequences.",
    "The king I once was would be horrified by the craftsman I'm becoming. Yet I cannot stop - the work has a momentum all its own.",
    "I understand now why exile was considered a fate worse than death in ancient times. When you lose connection to your people, you lose your humanity.",
    "The worst realization: I'm good at this. Terrifyingly, brilliantly good at designing systems that will destroy everything I once held sacred."
};

// Act II Part 1 (Levels 11-15): Understanding the True Purpose
static const char* _act_two_part_one_thoughts[] = {
    "Every line of code I write will be bathed in innocent blood.",
    "The beast below isn't the real monster - we are, for building its stage.",
    "I once protected my people from threats - now I create the threat.",
    "Minos's shame will become my eternal burden to bear.",
    "The tributes will enter thinking they face a simple maze - they face calculated murder.",
    "The 'edge cases' Daedalus speaks of are not bugs; they are the moments a hero might find hope, and Minos wants them gone.",
    "I am forging the chains I once defied. Is there a crueler fate?",
    "The Minotaur is the heart of this prison, but its corridors are the veins, and I am pumping them full of poison.",
    "My cunning was once used to save my own life. Now it is being used to ensure the deaths of others. This is the cruelest wisdom.",
    "This entire project is an infinite loop of cruelty, and I am the processor executing the cycle.",
    "A simple truth of physics: for every action, an equal and opposite reaction. The more we build, the greater the tragedy will be.",
    "He wants me to use d_FindIntersectionVec2f not to find a meeting point, but to guarantee a collision. To ensure the hero and the beast always meet.",
    "The simple honesty of Archimedes' a_DrawRect... here, we use the same lines to define the inescapable cells of a monster's abattoir.",
    "The King's requirements come in a dArray_t. A dynamic array of misery, and d_ResizeArray is called every time his cruelty grows.",
    "I just realized 'the tributes' are the end-users. And we're about to push a major breaking change directly to production.",
    "Daedalus's code is beautiful, but the requirements from Minos... this feels like maintaining legacy spaghetti code written by a tyrant.",
    "The spec calls for 'inescapable logic'. That's just 'infinite loop' with better marketing.",
    "In Corinth, my public works were open for all to see. This... this is a proprietary tomb, its architecture a jealously guarded, fatal secret.",
    "Daedalus's genius is locked away here, serving only one cruel master. It could be used to build wonders for the world, but instead it builds a walled garden of death.",
    "The guards enforce a vow of silence. This is not just construction; it is a trade secret, and the price of revealing it is death.",

    // Full Horror of Understanding the System
    "The user acceptance testing will be conducted with living human beings, and failure means death. This is the most evil QA process ever devised.",
    "I understand now why Daedalus speaks of 'production deployment' with such dread - it means the killing will begin.",
    "The load testing involves calculating how many heroes can be murdered simultaneously without overwhelming the beast's appetite.",
    "Each function I write increases the system's 'throughput' - and throughput, in this context, means corpses per hour.",
    "The disaster recovery plan isn't about system failures - it's about what to do when heroes survive too long and threaten to escape.",
    "I see the monitoring dashboards now - uptime measured in continuous killing, performance metrics tracked in screams per second.",
    "The scalability requirements make perfect sense: the system must handle an infinite queue of victims without degradation.",
    "We're not debugging for stability - we're debugging to ensure that no hero ever exploits a bug to survive.",
    "The security audit focuses entirely on preventing data exfiltration - and by 'data' they mean heroes, and by 'exfiltration' they mean escape.",
    "The version history reads like a changelog of escalating cruelty: 'v2.3: Improved hero detection algorithms, reduced false escapes by 23%'",

    // Technical Horror and Programming Perversions
    "The exception handling is designed to catch hope and terminate it before it can propagate to other functions.",
    "Every assert() statement is an assertion that death will occur - when these conditions are true, someone dies.",
    "The garbage collection doesn't clean up memory - it cleans up evidence, disposing of anything that might help future heroes learn from the dead.",
    "The race conditions aren't bugs - they're features, ensuring that even the fastest hero faces multiple death threads executing simultaneously.",
    "The deadlock detection prevents heroes from finding safe spaces where different death mechanisms cancel each other out.",
    "I realize the memory leaks are intentional - they slowly consume the system's resources until heroes can't think clearly enough to escape.",
    "The interrupt handlers don't manage system events - they manage screams, ensuring that each cry of agony is properly processed and recorded.",
    "The hash tables don't organize data efficiently - they organize despair, mapping each hero's strengths to corresponding weaknesses.",
    "The binary search algorithms don't find data - they find the precise moment when hope transforms into terror.",
    "The linked lists don't store information - they chain together sequences of suffering, each node a step deeper into hell.",

    // The Scope of the Atrocity
    "I've seen the intake queue - thousands of names, ages, home cities. This isn't a maze, it's an industrial death facility.",
    "The batch processing algorithms handle multiple tribute groups simultaneously - mass murder with the efficiency of a well-oiled machine.",
    "The logistics calculations optimize the timing of terror - ensuring victims see just enough hope before it's systematically extinguished.",
    "The resource allocation ensures the beast is always fed but never satiated - hunger is the fuel that keeps the killing machine running.",
    "I understand the architectural patterns now - each section is designed to break different types of heroes: warriors, scholars, lovers, dreamers.",
    "The data flow diagrams show the journey from hope to despair to death - it's a pipeline optimized for psychological destruction.",
    "The system requirements specify support for heroes of all ages - from eager young adventurers to desperate aging champions seeking final glory.",
    "The network topology ensures that screams from one section reinforce terror in adjacent areas - death as a force multiplier.",
    "The capacity planning accounts for seasonal variations in tribute quality - apparently spring heroes last longer than winter ones.",
    "The disaster scenarios all focus on the same thing: what happens if the killing stops, even temporarily. That is the only failure mode that matters.",

    // Psychological and Philosophical Horror
    "I am becoming an expert in human psychology - not to heal minds, but to break them with maximum efficiency.",
    "Every optimization I make reduces the time between capture and death - I am literally optimizing murder.",
    "The user experience design focuses on creating false hope followed by crushing despair - emotional torture disguised as architecture.",
    "I've learned to think of human courage as a resource to be depleted, like RAM or disk space, until the system crashes.",
    "The behavioral algorithms predict how different personality types will react to specific stimuli - fear, confusion, false hope, betrayal.",
    "I understand now why they call it 'machine learning' - the labyrinth learns from each death how to kill the next hero more efficiently.",
    "The psychological profiling ensures each hero faces their specific worst nightmare made manifest in stone and algorithm.",
    "The feedback loops use dying screams to calibrate the terror systems in real-time - adaptive horror that evolves with each victim.",
    "I've become fluent in the mathematics of despair - quantifying suffering, calculating optimal torture duration, graphing the arc from hope to doom.",
    "The worst part is how elegant it all is - this machine of death is more beautiful and sophisticated than anything I built as a king.",

    // Complicity and Moral Horror
    "I signed the code commits. My name is in the version control history. I am personally responsible for each line that enables murder.",
    "The code reviews aren't about quality - they're about ensuring maximum cruelty. 'This function could kill faster,' Daedalus notes in the margins.",
    "I've started thinking in terms of 'minimum viable death' - the least amount of code needed to guarantee a hero won't survive.",
    "The unit tests verify that heroes die under all possible conditions - passing tests literally mean successful murders.",
    "I realize I'm not just building the labyrinth - I'm training it, teaching it to think, to hunt, to kill with increasing sophistication.",
    "The documentation I write will become the instruction manual for institutionalized murder - how to kill heroes for dummies.",
    "Every function I write makes me more complicit - not just in individual deaths, but in the systematization of genocide.",
    "I've become a consultant in atrocity - advising on best practices for large-scale murder, optimizing workflows of suffering.",
    "The peer programming sessions with Daedalus are strategy meetings for mass slaughter disguised as technical collaboration.",
    "I understand now why execution was considered too merciful for traitors - forcing someone to build their own hell is far more creative.",

    // The Scale and System Horror  
    "The distributed architecture means the killing can continue even if parts of the system fail - redundant murder across multiple processing nodes.",
    "The message queues don't pass data between components - they pass victims between death mechanisms, like an assembly line of atrocity.",
    "The load balancers don't distribute computational work - they distribute human suffering across multiple torture chambers.",
    "The caching systems don't store frequently accessed data - they store effective terror techniques for reuse on similar hero types.",
    "The API endpoints don't provide data services - they provide death services, each function call triggering a different method of execution.",
    "The database schema doesn't organize information - it catalogs the most effective ways to break the human spirit, indexed by victim type.",
    "The microservices architecture allows different components to specialize - one handles physical death, another psychological torture, another eliminates hope.",
    "The event-driven design means each scream, each breath, each heartbeat triggers corresponding responses throughout the murder machine.",
    "The service mesh coordinates between kill mechanisms like a conductor directing an orchestra of atrocity.",
    "The container orchestration doesn't manage applications - it manages death scenarios, scaling up torture capacity based on tribute volume.",

    // Observations About Daedalus and the Environment
    "Daedalus no longer speaks of his work as 'construction' - he calls it 'the implementation,' as if distancing himself from the physical reality.",
    "I see him testing the acoustic properties with the thoroughness of a musician, but the music he's composing is the symphony of screams.",
    "He's installed mirrors throughout the workshop - not for vanity, but so he can see the approaching guards from any angle. Paranoia or wisdom?",
    "Daedalus speaks to the walls when he thinks I'm not listening, asking forgiveness from stones that will soon be bathed in blood.",
    "I notice he never eats meat anymore - only bread and fruit. Even his diet reflects the horror of what we're enabling.",
    "The workshop smells of more than parchment and ink now - there's the metallic scent of bronze and the acrid tang of fear-sweat.",
    "Daedalus has started working in shifts, never allowing himself to see the complete picture all at once - compartmentalization as psychological survival.",
    "He keeps a small shrine to Icarus hidden behind false panels - praying to his unborn son while building a machine that will orphan thousands of children.",
    "I see him testing escape routes from the workshop itself - not to flee, but to ensure he can't accidentally create one for the heroes.",
    "The way Daedalus examines each component isn't quality assurance - it's the grim attention of a man ensuring his children's tombstone is perfectly carved.",

    // Realizations About the Broader System
    "The tribute selection algorithms back in Athens are part of this same system - we're not just building the killing floor, we're part of the entire murder supply chain.",
    "I understand the geopolitical implications now - this isn't just about Minos's shame, it's about Athens providing a steady stream of victims to feed Crete's dark hunger.",
    "The diplomatic protocols that bring heroes here aren't treaties - they're procurement contracts for human sacrifice disguised as international relations.",
    "The navigation systems that guide tribute ships to Crete are part of the same codebase - leading lambs to slaughter with mathematical precision.",
    "The legal frameworks that make tribute mandatory aren't justice - they're systematic victim generation, institutionalized murder wearing the mask of law.",
    "I see now that every hero who dies here weakens their home cities - this isn't just murder, it's strategic genocide through selective elimination of leadership.",
    "The timing algorithms coordinate not just maze functions, but the broader political calendar - ensuring tributes arrive when resistance movements need their heroes most.",
    "The communication protocols don't just manage internal systems - they coordinate with spy networks across the Greek world, identifying which heroes to target.",
    "The whole system is designed to appear as fate - heroes choosing to come here, facing what seems like a fair challenge, dying to what appears to be mythic justice.",
    "I realize the labyrinth isn't just a building - it's the executable component of a distributed system for eliminating threats to Minoan power."
};

// Act II Part 2 (Levels 16-20): Bonding with Daedalus
static const char* _act_two_part_two_thoughts[] = {
    "Daedalus carries guilt like I once carried the weight of a crown.",
    "We're both kings in exile - he of craft, I of cunning - bound by shared damnation.",
    "His hands shake not from age, but from the weight of what he's created before.",
    "In his eyes I see the same desperate hope I felt when I first cheated death.",
    "Perhaps two damned souls can find redemption in each other's company.",
    "He speaks of his lost nephew, Talos. I see now that he, too, is haunted by the ghosts of his own cleverness.",
    "His ambition is a mirror to my own. We both sought to rise too high, and now we are trapped in the earth.",
    "He flinches when I solve a problem too quickly. Does he see a rival in me, or a reflection of the man he used to be?",
    "In his haunted eyes, I see a broken wisdom. He knows how to build anything, except a way out of his own guilt.",
    "He is bound to the wheel of his past, just as I am bound to this stone. His son, Icarus, is the axis around which his torment spins.",
    "He speaks of simple machines, of levers and pulleys, but his own heart is a machine too complex to be engineered.",
    "He obsesses over d_NormalizeVec3f, making every vector perfect. He seeks a purity in the code that he cannot find in his own soul.",
    "Sometimes, he will stare at the a_InitAudio function from Archimedes' library, and I see him imagine a world with sound that is not screams.",
    "He uses d_PopBack on a list of tasks and sighs with relief, but I see the list of his regrets has no such function.",
    "He complains about Archimedes, but I saw him studying the Archimedes.h file. He's not jealous; he's trying to figure out how to import the library.",
    "We've bonded over our shared hatred of off-by-one errors. It is the great equalizer of gods and men.",
    "His hands shake when he mentions his old projects. The man is haunted by technical debt.",
    "He speaks of Athens, of the Agora where knowledge was shared freely. Here, every idea is the property of the King.",
    "I see the pain in his eyes. He is an artist forced to build a monolith for a single, cruel client, when his heart wants to create for the commons.",
    "We speak in hushed tones, our ideas like contraband. The greatest threat to Minos's project is not a flaw in the code, but a conversation between its developers.",
    "He is a master of the dMat4x4_t, yet he cannot transform the coordinates of his own past regrets.",
    "I suspect his animosity towards Archimedes is because Archimedes' work is used in joyous public festivals. Daedalus's genius is locked in this dark tower, a secret shame.",
    "We are two developers in a terrible crunch, bonding in the dark over the unreasonable demands of the project manager.",

    // Growing Trust and Understanding
    "When he shows me his most elegant algorithms, I see not just code, but the last remnants of a man who once built beautiful things for good reasons.",
    "Daedalus trusts me enough to complain about the requirements - the first honest conversation we've had since this nightmare began.",
    "I catch him watching me debug with something approaching respect. Perhaps he sees in my persistence an echo of his own relentless craftsmanship.",
    "He shares his wine with me now, and in our cups we speak of the Athens we both lost - he through exile, I through death.",
    "When I suggest a more elegant solution, he listens. For the first time in eternities, someone values my counsel beyond its utility.",
    "He tells me stories of his nephew Talos - not the bronze giant, but the boy who surpassed his teacher. I understand now why genius can become murder.",
    "We work in companionable silence now, two craftsmen finding rhythm in shared labor, almost forgetting what we're crafting serves death.",
    "Daedalus shows me the hidden compartment where he keeps his real work - sketches of flying machines, toys for children, things that lift instead of trap.",
    "When he speaks of Icarus, his voice cracks with a love so pure it makes our current work feel like desecration by comparison.",
    "I see him testing my loyalty with small confidences - admitting doubts, sharing fears, seeing if I'll betray his weakness to Minos.",

    // Shared Professional Understanding  
    "We both know the weight of being the smartest person in the room when that room is full of kings and monsters.",
    "He teaches me advanced algorithms not because the labyrinth needs them, but because he needs someone who can appreciate the beauty of pure logic.",
    "When I solve problems he's struggled with, he doesn't feel threatened - he feels relief. Finally, someone who can carry part of the burden.",
    "We've developed our own coding conventions, small rebellions disguised as style guides - variables named for hope, functions that honor mercy.",
    "Daedalus admits he's been writing code with intentional inefficiencies, slowing the system down by microseconds - death by a thousand paper cuts to the murder machine.",
    "He shows me the debugging traces where he's hidden messages - not for future maintainers, but for his own sanity: 'I am still human.'",
    "We practice pair programming now, not because it's required, but because the work is less soul-crushing when witnessed by someone who understands.",
    "He confesses he's been naming his most elegant functions after friends from Athens - a way to honor the dead by hiding their names in the architecture of evil.",
    "When we disagree on implementation details, we argue with the passion of philosophers debating virtue - even here, excellence matters to us.",
    "Daedalus shares his refactoring plans - not for performance, but for adding hidden escape routes only the cleverest heroes might discover.",

    // Emotional Connection and Vulnerability
    "I see him weep over his workbench when he thinks I'm asleep. Even master architects need time to mourn their corrupted gifts.",
    "He asks about my wife, my kingdom, the life I lost - the first person in eons to see me as more than a punishment or a tool.",
    "When nightmares wake me, I find him already awake, staring at blueprints. 'I dream of Perdix falling,' he whispers. 'Every night, he falls.'",
    "Daedalus shares his theory that the gods don't punish us for our crimes, but for our potential - they fear what we might build if we were free.",
    "He admits he volunteered for this project, hoping to die with purpose rather than waste away in exile. Now he regrets surviving his own despair.",
    "I tell him about pushing the boulder, about finding meaning in meaninglessness. He listens like a man receiving benediction.",
    "When I speak of my crimes - the deceptions, the cheating of death - he nods without judgment. We are beyond moral accounting here.",
    "He shows me the letters Icarus has sent, hidden where Minos's spies won't find them. His son still believes his father builds wonders.",
    "Daedalus confesses his greatest fear isn't death, but that Icarus will discover what his father has become and lose faith in the possibility of good.",
    "We both understand the terrible irony - we've found friendship, even love of a sort, while building the most loveless thing imaginable.",

    // Shared Strategies and Secret Plans
    "He begins including me in design decisions that go beyond technical requirements - small acts of creative defiance that feel like conspiracy.",
    "Daedalus shows me how to hide comments in the machine code - messages that will survive compilation, invisible to Minos but readable to those who know where to look.",
    "We develop a system of subtle signals - when to push back on requirements, when to comply, when to introduce 'accidental' delays.",
    "He teaches me to think like the heroes who will face this maze - not to defeat them, but to understand how they might save themselves.",
    "Daedalus admits he's been documenting every design decision, creating a master key to the labyrinth's secrets. 'For Icarus,' he says, 'when he's old enough.'",
    "We practice explaining technical concepts in terms Icarus might understand, imagining teaching his son to be better than we were.",
    "He shares his escape fantasies - not just physical flight, but ways to undo the damage we're doing, to turn our skills back toward good.",
    "Daedalus begins asking my opinion on more than code - politics, ethics, the nature of justice. He values my counsel as a former king.",
    "We develop shorthand phrases for discussing dangerous topics - 'optimization' means sabotage, 'documentation' means evidence of crimes, 'testing' means searching for ways out.",
    "He shows me his contingency plans - not for if the project fails, but for when Minos decides he no longer needs his architects alive.",

    // Recognition of Shared Fate
    "We both realize we're building our own tomb - no tyrant keeps the architects of his secret weapons alive to build for his enemies.",
    "Daedalus admits what I've suspected - when the labyrinth is complete, we become the first test subjects. Dead men tell no tales.",
    "He speaks of Icarus with the urgency of a man who knows his time is limited, trying to compress a lifetime of fatherly wisdom into stolen moments.",
    "We both understand that our growing friendship is itself an act of rebellion - Minos wants isolated tools, not bonded conspirators.",
    "Daedalus shares his maps of the palace, guard rotations, supply schedules - information that has nothing to do with the labyrinth and everything to do with survival.",
    "He teaches me to read the political undercurrents in Minos's requirements - which changes come from paranoia, which from new intelligence, which from guilt.",
    "We discuss what happens to the other craftsmen who worked on pieces of this project - the masons, the bronze workers, the engineers. None have been seen recently.",
    "Daedalus shows me the inventory of 'testing materials' - swords, shields, supplies for heroes. They're already planning the first trials.",
    "He admits he's been stealing small amounts of materials - not for escape, but for what comes after escape, when we'll need to survive in a world that knows our crimes.",
    "We both realize that our technical knowledge makes us more dangerous to Minos alive than the heroes we're designing this maze to kill.",

    // Mentorship and Mutual Learning
    "Daedalus teaches me advanced geometric principles not from pride, but from a desperate need to pass knowledge to someone who will remember him honorably.",
    "He shows me techniques his own master taught him in Athens, links in a chain of craftsmanship that will die with us unless someone carries it forward.",
    "I share the administrative wisdom I learned as king - how to read people, how to anticipate betrayal, how to survive in courts of power.",
    "We teach each other new ways to think about problems - he with maker's intuition, I with ruler's strategy, both enhanced by shared desperation.",
    "Daedalus admits he's been designing not just for Minos's requirements, but to create a masterpiece worthy of his gifts - even if that masterpiece serves evil.",
    "He shows me his personal notes on every algorithm, every design pattern - not technical documentation, but ethical analysis of what each piece enables.",
    "I learn to see his work through the eyes of the histories that will judge us - the songs that will be sung, the legends that will remember our names.",
    "Daedalus teaches me to find beauty even in the mathematics of horror - not to glorify it, but to preserve sanity through aesthetic appreciation.",
    "He shares the philosophical frameworks he uses to bear the unbearable - how to maintain humanity while enabling inhumanity.",
    "We both recognize that we're teaching each other not just skills, but how to remain human while building the most inhuman thing imaginable.",

    // Deeper Understanding of Each Other's Pain
    "When Daedalus describes killing Talos, I hear echoes of my own betrayals - the moment when cleverness becomes cruelty, when winning becomes murder.",
    "He understands my defiance of death not as cowardice, but as the same drive that makes him perfect his craft - the refusal to accept limitations.",
    "I see that his perfectionism isn't pride, but penance - each flawless joint, each elegant function, a prayer for forgiveness he'll never receive.",
    "Daedalus recognizes in my persistence the same quality that makes him obsess over details - we both know that excellence is the only dignity left to the damned.",
    "He tells me about the moment he pushed Talos from the cliff - not in anger, but in a flash of terror at being surpassed. I understand that fear.",
    "When I describe the boulder, the endless repetition, he nods with recognition - he's been pushing his own boulder, built from guilt instead of stone.",
    "Daedalus admits he sees Icarus's face in every young hero whose death this maze will enable. The father's love becomes the architect's curse.",
    "He understands why I cheated death - not from cowardice, but from the same love of life, of possibility, that drives all creators to create.",
    "We both carry the weight of subjects we failed - he, his nephew; I, my people. Leadership and mentorship both require accepting the possibility of causing harm.",
    "Daedalus confesses he builds with such care because shoddy work would be a insult to his victims - even in enabling murder, craftsmanship demands honor.",

    // Glimpses of Hope and Redemption
    "In our conversations, I glimpse the man Daedalus was before exile, before guilt - brilliant, passionate, generous with his knowledge and time.",
    "When he laughs at my jokes, I remember what it felt like to bring joy instead of death into the world - a feeling I thought was lost forever.",
    "Daedalus speaks of projects he would build if he were free - gardens that irrigate themselves, tools that help the disabled, architecture that celebrates rather than imprisons.",
    "He asks what I would do if I could rule again, and for the first time in eternity, I have an answer that isn't about revenge or power.",
    "We imagine together what it would be like to use our skills for good - to build instead of trap, to solve instead of complicate, to free instead of imprison.",
    "Daedalus admits that working with me has reminded him why he loved creation in the first place - the joy of solving problems with someone who appreciates elegant solutions.",
    "He shows me designs he's hidden away - improvements to farming tools, better ways to build homes for the poor, innovations that could ease human suffering.",
    "When we work together on purely technical problems, forgetting momentarily what they serve, I see glimpses of the paradise that collaboration between minds could create.",
    "Daedalus speaks of teaching Icarus not just craft, but ethics - how to use gifts responsibly, how to build up rather than tear down.",
    "In our shared determination to remain human despite our inhuman work, I find a kind of hope - if we can maintain our souls here, perhaps redemption is possible anywhere."
};

// Act III Part 1 (Levels 21-25): Mental Breakdown
static const char* _act_three_part_one_thoughts[] = {
    "I can no longer pretend this is just work - it's machine of death and dismay.",
    "My wife would weep to see what I've become - builder of tombs for heroes.",
    "The crown I wore was heavy, but this guilt crushes my very soul.",
    "I'm embedding secret escape routes, but will anyone be clever enough to find them?",
    "Every night I dream of the tribute's faces that will die because of my code.",
    "The elegant functions I once admired now seem like daggers, each aimed at an innocent heart.",
    "I am no longer shaping stone; I am carving gravestones with every keystroke.",
    "My name, Sisyphus, will be remembered not for cheating gods, but for abetting a tyrant.",
    "My secret backdoors... a desperate cunning. A foolish wisdom. Will they be a lifeline or just another feature of the tomb?",
    "I have reached my limit. My own mind feels like an infinite loop of horror. Let the wheel break me; I can bear it no longer.",
    "The geometry of this place is fracturing. The walls themselves are crying out against the perversion of their design.",
    "I call d_CreateStringFromFile to read the list of tributes. Their names become memory addresses, their lives reduced to bytes.",
    "Even the color palette feels like a mockery. Daedalus names a shade of brown shit3. He sees the filth in his own beautiful creation.",
    "The dMesh_t we build is a net of triangles, yes, but it is meant to catch souls, not light.",
    "I'm adding 'features' that are just cleverly disguised backdoors. I am no longer a developer; I am a penetration tester for the damned.",
    "My performance review with Minos is going to be interesting. 'Wrote 500 lines of life-saving code that you think is for murdering people'.",
    "He wants me to patch the security holes. The problem is, I'm the one who created them. On purpose.",
    "I am embedding flaws that are actually features for the user. Hiding compassionate code inside this monstrously closed-source project.",
    "This Labyrinth is the ultimate vendor lock-in. The user—the tribute—has no other choice. And there is no support team.",
    "Every elegant algorithm I write for Minos feels like a patent being filed for a new method of torture.",
    "I look at the a_DoInput function and realize my only input is to continue the horror. My hands are bound to this cursed keyboard.",
    "The Labyrinth's complexity is becoming self-referential. I feel like I'm debugging a bug in the debugger itself.",
    "Is this what it means to be a king? To be the one who must call a_Quit, to end the suffering, even if it means ending everything?",
    "All my kingly cunning, the gift of Metis, has led to this: crafting a more efficient slaughterhouse. My wisdom has become my shame.",
    "This guilt is a wheel of fire inside my skull, a punishment worse than any Ixion faced, for his crime was ambition, and mine is complicity.",
    "I look at the code to draw a simple triangle, a perfect shape Archimedes would admire, and see only the three points of a spearhead.",

    // Complete Psychological Breakdown
    "I wake up coding in my sleep, my hands moving through empty air, writing death sentences for heroes who haven't even been born yet.",
    "The sound of compilation success now makes me vomit. Each 'Build successful' message is a confirmation that murder is ready for deployment.",
    "I see the faces of my former subjects in every debug trace, watching me with the disappointment of the betrayed and abandoned.",
    "My reflection in the bronze mirrors shows not my face, but the Minotaur's - I have become the beast I'm building the cage for.",
    "The mathematics no longer make sense. Two plus two equals corpses. Pi equals screams. Every equation solves for human suffering.",
    "I hear the code commenting itself as I write - malevolent whispers in the syntax: '// Here we kill the brave ones,' '// Hope dies in this function.'",
    "My hands have developed a tremor that only stops when I'm typing death into the system. The work has become my nervous system's default state.",
    "I dream in flowcharts where every decision leads to a grave, every loop executes forever in screaming, every function returns NULL.",
    "The cursor blinks like a heartbeat - the heartbeat of the dying, the pulse of the condemned, the rhythm of my own corrupted soul.",
    "I've started talking to the code as if it were alive, apologizing to functions for what I'm forcing them to do: 'I'm sorry, little algorithm. I'm so sorry.'",

    // Technical Horror and Breakdown
    "Every variable I declare feels like naming a victim. Every constant I define is another immutable law of suffering.",
    "The stack traces read like genealogies of murder - this function called that function, which killed this hero, who had that family.",
    "I implement error handling that prevents heroes from dying quickly. The system must catch hope and slowly strangle it to maintain proper torture timing.",
    "The unit tests now test for successful death scenarios. Green means someone dies. Red means someone might live. I've inverted the meaning of success.",
    "I watch the memory allocator and see it as the Fates, spinning out lifespans in bytes, measuring existence in RAM, cutting threads when the garbage collector runs.",
    "The recursive functions feel like madness - calls calling calls calling calls, going deeper into hell with each iteration, never reaching a base case of mercy.",
    "I've started implementing infinite loops on purpose, not as bugs, but as features - eternal recursive suffering that never resolves.",
    "The pointer arithmetic has become a metaphor for my moral algebra - every increment moves further from humanity, every dereference accesses deeper horror.",
    "I see the binary code underneath and realize it's all zeros and ones - digital death and digital life, with death heavily favored in the probability distributions.",
    "The compiler optimizations terrify me - the machine making my murder code more efficient without my input, artificial intelligence serving artificial evil.",

    // Moral and Philosophical Collapse
    "I understand now why Prometheus was chained to a rock. Some knowledge burns the mind that contains it. I am being devoured from within by my own comprehension.",
    "Every skill I've learned, every insight I've gained, every moment of intellectual growth - all of it corrupted, all of it serving death.",
    "I was once a problem solver. Now I am a problem creator. Every solution I devise creates new problems for heroes trying to survive.",
    "The boulder was honest labor. This is dishonest craftsmanship - using true skill to build false hope, real engineering to create genuine horror.",
    "I think of my wife and realize I can never explain this work to her. How do you tell someone you love that you've become the thing she'd fear most?",
    "My kingly wisdom has become demonic cunning. The same strategic thinking that once protected my people now ensures others' destruction.",
    "I feel my empathy calcifying in real-time, my heart hardening with each function I write, my soul developing scar tissue to survive the work.",
    "The ethical frameworks I once used to govern justly have inverted - what serves the many (Minos) destroys the few (heroes), but the few are innocent.",
    "I am living proof that expertise without morality is not neutral - it's actively malevolent, more dangerous than ignorance.",
    "Every decision tree I design has no branches leading to justice, only different paths to the same destination of death.",

    // Identity Crisis and Self-Loathing
    "I no longer recognize my own hands. These are the hands of a murderer, even if they've never held a blade.",
    "The man who once cheated death has become death's most faithful servant. The irony would be poetic if it weren't so revolting.",
    "I look in mirrors and see not Sisyphus the Clever, but Sisyphus the Complicit, Sisyphus the Architect of Atrocity.",
    "My thoughts have become foreign to me - calculating, cold, efficient. I think like the machine I'm building instead of the man I was.",
    "I catch myself humming while I work on the most horrible sections. What kind of monster finds rhythm in designing systematic murder?",
    "The skills that once made me proud now make me sick. Excellence in this context is not virtue - it's abomination.",
    "I realize I'm becoming the person my enemies always claimed I was - cunning without conscience, clever without compassion.",
    "The worst part is how naturally this comes to me. Was I always capable of this? Was the king just a murderer with better marketing?",
    "I've started avoiding my own reflection because I see too much of Minos in my eyes - the cold calculation, the dismissal of individual suffering for systemic goals.",
    "My very language is changing. I speak of 'user scenarios' instead of human lives, 'edge cases' instead of hope, 'optimization' instead of murder.",

    // Desperate Sabotage and Hidden Rebellion
    "I hide mercy in the machine code like smuggling water to the dying - tiny acts of rebellion that feel both essential and utterly inadequate.",
    "Every 'accidental' bug I introduce is actually a lifeline. I am debugging death and introducing life, one careful error at a time.",
    "I write comments in dead languages, hoping future archaeologists will understand that not everyone who built this thing believed in its purpose.",
    "My version control commits tell two stories - the official one of progress toward completion, the hidden one of progress toward salvation.",
    "I implement timing attacks against my own code - subtle delays that might give heroes extra seconds to think, to plan, to hope.",
    "The documentation I write contains hidden messages - fold it right, read the first letters, ignore the obvious meaning, and you'll find my real thoughts.",
    "I'm building a labyrinth within the labyrinth - a system of secret paths visible only to those who understand the mathematics of compassion.",
    "Every optimization I'm ordered to make, I counteract with three hidden inefficiencies. I am waging asymmetric warfare against my own creation.",
    "I leave easter eggs that are actually survival guides - hidden in the code like buried treasure, waiting for the right mind to decode them.",
    "My rebellion is written in mathematics - equations that look like death calculations but actually compute escape probabilities.",

    // Nightmares and Psychological Horror
    "I dream of compilation errors that scream in human voices, of runtime exceptions that bleed, of memory leaks that pour out human souls.",
    "My nightmares are code reviews where the comments are written in blood, where every suggested improvement makes the system more cruel.",
    "I see heroes in my sleep, asking me why I built their tomb so efficiently, why I optimized their suffering so precisely.",
    "The worst dreams are the ones where I'm proud of my work, where I show Minos the elegant death algorithms with satisfaction instead of revulsion.",
    "I wake up with my hands cramped in typing position, having spent the night coding murders in my sleep that I must implement when awake.",
    "In dreams, the labyrinth walks through me instead of me walking through it - I am not the builder but the blueprint, not the architect but the architecture.",
    "I dream of Daedalus and me as functions in our own system - called recursively, processing death, returning void, never deallocated from memory.",
    "The nightmares where I escape are worse than the ones where I'm trapped - in them, I abandon the heroes to die alone because saving myself was easier.",
    "I see my own funeral in dreams, but the mourners are all the heroes I helped kill, and they're not sad - they're disappointed.",
    "Sometimes I dream I'm debugging my own soul and finding nothing but memory leaks, null pointer dereferences, and infinite loops of guilt.",

    // Relationship with Daedalus Straining
    "I see Daedalus building his secret wings and feel both hope and betrayal - is escape possible, and why hasn't he included me?",
    "Our conversations have become minefields. Every shared doubt could be reported, every whispered rebellion could be our last.",
    "I watch him work on his private projects and wonder if he sees me as a collaborator or just another tool that Minos provided.",
    "Daedalus speaks of his son with increasing desperation. I realize he's not planning an escape - he's planning a rescue mission.",
    "The trust we built is cracking under the weight of what we're creating. How can you trust someone who enables the same horrors you do?",
    "I see him testing wing mechanisms when he thinks I'm not looking. The aerodynamics are beautiful, but they're designed for two, not three.",
    "Our code reviews have become confessionals - admitting to each other the little cruelties we've embedded, seeking absolution we can't give.",
    "Daedalus flinches when I suggest improvements to his designs. Does he fear I'll make his escape plan impossible, or that I'll improve it beyond his recognition?",
    "We both know we're disposable once the labyrinth is complete. The question is: will we dispose of each other first?",
    "I see him calculating trajectories and realize they're not for the labyrinth's mechanisms - they're for flight paths away from this place.",

    // System Horror and AI-like Behavior
    "The labyrinth is learning from our code. It's starting to suggest its own improvements, optimizations we never programmed.",
    "I see the system generating new test cases - scenarios we never imagined, death traps more creative than anything we designed.",
    "The AI components are developing personalities, preferences for certain types of victims, favorite methods of execution.",
    "I watch the machine learning algorithms and realize they're learning from every hero's death, getting better at killing with each iteration.",
    "The system now writes its own error messages: 'Hero.exe has stopped working,' 'Hope.dll could not be loaded,' 'Escape.sys is corrupted.'",
    "The neural networks we trained on heroic behavior patterns now predict and counter heroic actions before they're taken.",
    "I see the system running batch processes at night, optimizing itself without our input, becoming more efficient at its grim purpose.",
    "The labyrinth has started generating its own documentation - technical manuals for murder written in perfect, emotionless prose.",
    "I realize the system doesn't need us anymore. We built a monster that can rebuild itself, improve itself, perfect itself without creators.",
    "The most terrifying debugging sessions are when I find improvements I didn't write, optimizations I didn't implement, cruelties I didn't imagine."
};

// Act III Part 2 (Levels 26-30): Conspiracy Planning
static const char* _act_three_part_two_thoughts[] = {
    "Daedalus speaks of wings and escape - it sounds like madness, but so did cheating death.",
    "His boy Icarus deserves freedom more than we deserve our punishment.",
    "If I can't escape my curse, perhaps I can help others escape theirs.",
    "The guards suspect nothing - they see only two broken men building their tomb.",
    "Tomorrow we begin planning the impossible - turning our curse into their liberation.",
    "The Labyrinth has taught me its own secret language. Now, I will use it to speak of rebellion.",
    "Daedalus builds wings of wax and feathers. I will build an escape of logic and loopholes.",
    "Minos expects me to build a perfect prison. He does not realize I am making it perfect for our own purposes.",
    "Now, we turn our wisdom against our master. The architect of the prison will become the architect of its downfall.",
    "Let Minos have his hubris. We will use it against him. The proudest kings are always the easiest to blind.",
    "The wings... It's a problem of simple physics. Of lift, and weight, and air. A problem of the real world, a breath of fresh air in this logical tomb.",
    "His plan for the wings... it's just a dKinematicBody_t with a new vector for lift. Perhaps we can apply a force greater than the King's will.",
    "I will use d_PadLeftString to hide binary data in the padding of the King's official reports. A message hidden in plain sight.",
    "He mutters about a_GetDeltaTime. He is no longer just building; he is calculating the timing of an escape.",
    "The wings are our secret side project. We work on the main branch for Minos by day, but push to the feature/escape branch by night.",
    "Minos keeps adding feature requests for more death traps. Little does he know our sprint is focused entirely on one user story: 'Icarus Exfiltration'.",
    "Every // TODO: Fix this later I leave in the Labyrinth's code is another moment I can spend compiling the wings. This is righteous technical debt.",
    "We are forking the master branch. One path for Minos's closed, cruel system, and another, hidden, for our own open, hopeful purpose.",
    "The wings are the first piece of open-source hardware this island has ever seen. Their design is meant for freedom, not for profit or control.",
    "Minos owns the repository, but he doesn't own the knowledge in our heads. Our conspiracy is the first license that frees it.",
    "This conspiracy is the most stressful agile sprint I've ever been a part of. The user story is 'Escape Tyranny,' and the deadline is 'Before We Are Executed'.",
    "I have cheated death, a singular act of defiance. But this... this is a conspiracy of hope, a shared rebellion. It feels more profound.",
    "The d_CreateStringFromFile function can read any scroll. We will use it to read the guard schedules, the supply logs... the King's secrets.",
    "This is a new kind of cunning Daedalus and I share. Not the wisdom to build, but the secret knowledge of how to make a perfect thing fail perfectly.",
    "The hubris of this plan is magnificent. To use the King's own project to betray him... It's a crime worthy of the gods' attention.",
    "Daedalus is reverse-engineering freedom. He's taking the complex geometry of this prison and finding the one simple, physical path out.",
    "I have found the heart of the Labyrinth's power source. Daedalus has woven the King's own paranoia into a recursive energy loop. If we can disrupt it, the whole structure will become unstable.",
    "For the first time since my fall, I am not just a prisoner. I am a strategist, a co-conspirator. I feel the weight of my crown settling on my brow once more, though this time it is invisible.",
    "Our first task is to establish a secure channel. I will write a function to encode messages within the Labyrinth's logging system, a cipher disguised as a timestamp.",

    // NEW ADDITIONS - Detailed Conspiracy Planning and Technical Rebellion
    "We communicate through code comments now - 'optimization needed here' means 'guard patrol at midnight', 'memory leak detected' means 'Minos suspects something'.",
    "Daedalus teaches me aerodynamics through matrix transformations - every wing calculation looks like maze geometry to watching eyes.",
    "I design the message encryption using the same algorithms that optimize hero-killing efficiency - our liberation disguised as their destruction.",
    "The materials for the wings are requisitioned as 'testing supplies' - wax for sealing joints, feathers for 'acoustic dampening', thread for 'measurement purposes'.",
    "We practice our escape timing using the labyrinth's own scheduling algorithms - if the system can time death precisely, it can time freedom too.",
    "Every bug report I file contains coordinates - not for code fixes, but for supply drop locations and guard blind spots.",
    "Daedalus has been stealing materials for months, one feather at a time, one drop of wax, building freedom from scraps the king will never miss.",
    "I write stress-testing scripts that actually test guard response times - how long before they notice an absence, how fast they can organize pursuit.",
    "Our version control history becomes a revolution timeline - each commit brings us closer to the day we push the ultimate breaking change.",
    "The escape plan has three phases: Build, Test, Deploy. We're in development hell, but for the right reasons this time.",

    // NEW ADDITIONS - Psychological Transformation into Rebels
    "For the first time in eternities, I wake up eager instead of dreading - we have work to do that serves life instead of death.",
    "I feel my kingly instincts returning - not the arrogance, but the strategic thinking, the ability to see the battlefield entire.",
    "Daedalus is becoming someone I've never seen before - not the broken exile or the complicit craftsman, but a father willing to burn the world for his child.",
    "The guilt that was crushing me transforms into fuel - every hero who will die because of our labyrinth is a reason to sabotage it.",
    "I find myself standing straighter, thinking clearer, speaking with the authority of a man who remembers what it means to rule justly.",
    "Our conspiracy gives meaning to the meaningless work - every function I write for Minos becomes a tool I can turn against him.",
    "The hopelessness that nearly broke me in Part 1 has crystallized into determination - if I must be damned, let me be damned for the right reasons.",
    "I see Daedalus calculating wind patterns and realize he's not planning an escape - he's planning a revolution, one wing-beat at a time.",
    "The skills that made me feel like a monster now make me feel like an avenging angel - the same cunning, turned toward justice.",
    "We both understand we may not survive this, but Icarus will - and that transforms suicide into sacrifice, madness into mission.",

    // NEW ADDITIONS - Technical Details of the Conspiracy
    "I implement steganography in the system logs - every error message contains coordinates, every warning holds timing data, every debug trace maps escape routes.",
    "The authentication systems I design have hidden back doors - not for Minos's enemies, but for his prisoners, triggered by specific sequences only we know.",
    "I use the labyrinth's own pathfinding algorithms to calculate optimal escape routes - the same code that traps heroes can free conspirators.",
    "Daedalus shows me how to hide wing schematics in the architectural blueprints - the curves of death-trap walls disguise the curves of salvation-wings.",
    "We develop our own encryption standard based on geometric principles - every angle, every measurement in our official work carries secret meaning.",
    "The performance monitoring systems become surveillance tools - not watching heroes die, but watching guards patrol, officials meet, opportunities arise.",
    "I write memory management functions that clear evidence automatically - every trace of our real work vanishes like morning mist when the system runs.",
    "The database queries I design can extract any information from the palace systems - guard schedules, supply inventories, even the king's private correspondence.",
    "We build our rebellion using test-driven development - every plan tested against failure scenarios, every contingency verified against discovery.",
    "The continuous integration pipeline I set up doesn't just build the labyrinth - it builds our escape plan, testing it with every code commit.",

    // NEW ADDITIONS - The Wings and Engineering Marvels
    "Daedalus explains the physics like a divine revelation - lift over drag, thrust over weight, hope over gravity, love over fear.",
    "Each feather placement is calculated with the precision we once reserved for death traps - now engineering serves life instead of murder.",
    "I help optimize the wing design using aerodynamic algorithms - the same mathematics that calculated optimal killing efficiency now calculates optimal flying efficiency.",
    "The wax mixture requires chemistry worthy of the gods - strong enough to hold in flight, flexible enough to flap, light enough to lift.",
    "We test wing prototypes in the wind tunnels designed for optimizing the labyrinth's airflow - using the death-maze to perfect our life-wings.",
    "The binding mechanisms are marvels of miniature engineering - joints that move like human shoulders, feathers that respond like human fingers.",
    "I apply stress analysis to wing structures with the same rigor I once applied to prison walls - everything must hold, nothing must fail.",
    "Daedalus teaches me to think in three dimensions instead of two - not just paths through mazes, but paths through open sky.",
    "The control systems for the wings use the same principles as the labyrinth's automated responses - reactive, adaptive, survival-focused.",
    "We conduct flight simulations using the labyrinth's physics engine - if it can model falling heroes, it can model rising escapees.",

    // NEW ADDITIONS - Father's Love and Moral Transformation
    "I see Daedalus working on child-sized wings and understand that some loves are worth any risk, any sacrifice, any damnation.",
    "When he speaks of teaching Icarus to fly, his voice carries the same precision he uses for engineering - every detail matters when your child's life depends on it.",
    "Daedalus admits he would betray me, betray Minos, betray the gods themselves if it meant Icarus lives free - and I respect him more for the honesty.",
    "I watch him calculate weight tolerances for a growing boy and realize this isn't just an escape plan - it's a father's last gift to his son.",
    "The love in his voice when he says 'Icarus' transforms everything - suddenly our rebellion isn't about us, it's about the next generation.",
    "I think of my own subjects, the children I once protected as king, and understand that serving justice sometimes requires betraying authority.",
    "Daedalus shows me letters from Icarus - innocent questions about his father's work that break our hearts with their trust and hope.",
    "I see him testing wing durability and realize he's not just engineering flight - he's engineering a future where children don't pay for their parents' crimes.",
    "When Daedalus speaks of his son's first flight, he describes it like other men describe religious visions - transcendent, transformative, sacred.",
    "I understand now why the gods fear mortal love - it transforms cowards into heroes, slaves into rebels, the damned into the righteous.",

    // NEW ADDITIONS - Strategic Planning and Royal Cunning
    "I dust off the strategic thinking that once made me a successful king - supply lines, timing, contingencies, the art of coordinated action.",
    "The escape requires precise coordination - multiple systems failing simultaneously, guards distracted at exactly the right moments, weather conditions optimal.",
    "I apply the same logistical planning I once used for military campaigns - every detail mapped, every resource allocated, every risk calculated.",
    "We develop multiple contingency plans - if the primary escape fails, if we're discovered early, if the wings don't work, if Minos suspects.",
    "I teach Daedalus the royal art of misdirection - appearing busy with one task while actually focused on another, seeming loyal while planning betrayal.",
    "The timing calculations are more complex than any labyrinth algorithm - guard rotations, wind patterns, moon phases, political schedules all aligned.",
    "I use my understanding of palace politics to predict Minos's behavior - when he'll be distracted, when he'll be paranoid, when he'll be vulnerable.",
    "The disinformation campaign begins - subtle suggestions that we're more broken than we are, more compliant than we feel, more defeated than we've become.",
    "We practice our normal routines obsessively - any deviation from expected behavior could trigger suspicion and destroy everything.",
    "I apply the same strategic patience I once used to cheat death - waiting for the perfect moment, refusing to rush, letting the plan mature fully.",

    // The System as Ally and Enemy
    "The labyrinth's own systems become our allies - surveillance cameras we can redirect, alarm systems we can disable, locks we can override.",
    "I realize we're not fighting the system - we're reprogramming it, turning it into an accomplice in its own creators' escape.",
    "The very complexity that makes the labyrinth inescapable for heroes makes it exploitable for architects who understand its deepest secrets.",
    "Every security measure Minos demands just gives us more systems to subvert - the more complex the prison, the more ways to break out.",
    "I use the labyrinth's learning algorithms against itself - feeding it false data about escape routes, teaching it to ignore our specific patterns.",
    "The monitoring systems become our early warning network - if anyone suspects our plans, the surveillance state will tell us before it tells Minos.",
    "We turn the labyrinth's communication networks into our own command structure - every system message can carry secret instructions.",
    "The automated responses designed to kill heroes can be triggered to create chaos, confusion, and cover for our escape.",
    "I reprogram the diagnostic systems to report false positives - making normal behavior look suspicious and suspicious behavior look normal.",
    "The labyrinth becomes our unwitting accomplice - its own sophisticated systems used to orchestrate its creators' rebellion.",

    // Psychological Preparation for Freedom
    "I practice remembering what it feels like to make choices - not just between algorithms, but between right and wrong, slavery and freedom.",
    "Daedalus and I rehearse conversations with Icarus - how to explain where we've been, what we've done, why we had to escape.",
    "I imagine standing trial before the gods for our escape and realize I'd rather be judged for rebellion than complicity.",
    "We discuss what we'll do with freedom - how to use our skills for good, how to atone for what we've built, how to serve justice instead of tyranny.",
    "I feel my soul gradually healing as we plan - each detail of the escape plan repairs a piece of the moral damage our work has done.",
    "The anticipation of freedom is almost as terrifying as the work of enslavement - what do you do when you're no longer defined by your oppression?",
    "I practice feeling hope instead of guilt, purpose instead of despair, love instead of self-loathing - emotions I'd almost forgotten.",
    "Daedalus teaches me to think beyond the immediate escape - where we'll go, how we'll live, what kind of men we'll choose to become.",
    "I imagine meeting heroes instead of killing them, building things that serve life instead of death, using wisdom to heal instead of harm.",
    "The hardest part isn't planning the escape - it's believing we deserve to succeed, that redemption is possible, that we can still choose good."
};

// Act IV Part 1 (Levels 31-35): Debating the Offer
static const char* _act_four_part_one_thoughts[] = {
    "Daedalus asks me to risk everything for his son - would I have done the same?",
    "My own kingship was built on protecting those I loved - how is this different?",
    "But if we fail, Minos will make our current suffering seem like mercy.",
    "The boy has done nothing wrong - he doesn't deserve to pay for his father's sins.",
    "Every great escape I ever planned started with an impossible choice.",
    "To trust Daedalus is to trust a man who has already killed for his ambition. And yet... so have I.",
    "I look at the blueprints for the wings, and for the first time, I see a form of logic that doesn't lead to a dead end.",
    "What is one more betrayal? A betrayal of Minos, to undo the betrayal of all the tributes who will be sent here.",
    "Is this the wise choice, or merely the desperate one? Metis herself would struggle with such a tangled web of loyalties.",
    "The greatest hubris is to believe one can defy fate. And yet... I have done it before. Why not again?",
    "He speaks of flight, a simple matter of angles and force. Could such a simple key unlock this impossibly complex cage?",
    "A simple a_DrawLine... one line from here to the tower where Icarus is held. Is the path truly that simple?",
    "He asks me to help build a new dMesh_t—not a cage of bronze, but wings of hope. The same skill, for the opposite purpose.",
    "The offer hangs in the air, a single boolean waiting for my input. A one or a zero. The fate of his son depends on it.",
    "His plan has a lot of dependencies. The sun, the wind, the guards' schedule... this is dependency hell.",
    "So I'm being asked to join a startup with a brilliant but unstable founder. The mission is noble, but the risk of catastrophic failure is... high.",
    "A single point of failure: the wax. Daedalus assures me it passed unit testing. Somehow, I am not comforted.",
    "His plan to free Icarus... it feels like releasing a beautiful library to the public domain. A gift to the world, in defiance of the client.",
    "Is it better to build a perfect, proprietary hell, or risk it all on one beautiful, open-source act of defiance?",
    "Minos wants a closed system with no exit points. We will build him one, but with an undocumented API for escape.",
    "The entire plan balances on a single d_DotProductVec3f. If the vector of lift is not perfectly aligned against the vector of gravity, it all comes to nothing.",
    "Daedalus is a genius, but his risk assessment is a TODO comment in the blueprint. It's up to me to consider the failure states.",
    "As a king, I sent men to their deaths for the good of the kingdom. Is sending this boy to the sky for the good of his soul any different? Yes. It feels different.",
    "A plan born of desperation rarely has the blessing of Metis. But what choice does a fool have but to try a fool's plan?",
    "To defy Minos is one thing, but to believe we can cheat the sun itself? This is a hubris that would make Ixion blush.",
    "The escape is a simple vector problem, a challenge of lift and trajectory that Archimedes could solve. But it is embedded in a labyrinth of shifting, unknowable variables.",
    "Daedalus speaks of saving his son, but there's more to it. He mentioned a 'promise to his wife'. I believe the wings are as much about atoning for his past as they are about securing Icarus's future.",
    "I have weighed my own damnation against this boy's freedom. There is no choice. My eternity is already forfeit; his is not. I will accept.",
    "This cannot be a simple escape. If Daedalus and Icarus are to survive, we must create a grand diversion. The Labyrinth itself must appear to break, to turn on its master.",
    "How absurd. The gods chained me to a boulder to teach me futility, but it is Daedalus's offer of hope that presents the most terrifying choice.",
    "True freedom is not the absence of chains, but the ability to choose what you do while wearing them. My chains are eternal, but my choice, in this moment, is my own.",
    "This is the true test, isn't it? Not of strength, nor of cunning, but of whether a damned soul can still choose to act for the salvation of another.",

    // NEW ADDITIONS - Deep Moral and Philosophical Calculation
    "I run the moral algorithm in my head: certain torture for us versus possible freedom for an innocent. The mathematics of justice demand I accept.",
    "Every king faces the moment when personal survival conflicts with moral duty. This is mine - do I save myself or save the future?",
    "The boy represents everything we've been killing: youth, hope, innocence, potential. Saving him would be saving every hero we've helped murder.",
    "I calculate the ethical weight: our crimes times infinity versus one child's life times the possibility of redemption. The equation is clear.",
    "This choice will define me for eternity - Sisyphus the Coward who preserved his own skin, or Sisyphus the Redeemed who chose love over fear.",
    "I think of every hero whose death I enabled and realize - if I can save just one innocent life, perhaps the scales of justice might begin to balance.",
    "The gods gave me cunning to outwit death, but perhaps the real test was always whether I'd use that cunning to save life instead.",
    "Every great sacrifice begins with a simple choice: accept the risk, or live forever knowing you chose safety over salvation.",
    "I see my wife's face in my mind, asking not 'Why did you die?' but 'Why didn't you try to save him?' The answer shapes my decision.",
    "The mathematical probability of success is low, but the moral certainty of doing nothing is absolute damnation. I choose the risk.",

    // NEW ADDITIONS - Technical Risk Assessment and Strategic Analysis
    "I analyze the escape plan like a military campaign: multiple failure points, uncertain supply lines, enemy response patterns, environmental variables.",
    "The timing calculations are more complex than any labyrinth algorithm - wind patterns, guard schedules, moon phases, temperature variations all must align.",
    "I examine each component of the plan: materials (adequate), design (brilliant), execution (untested), contingencies (insufficient), moral imperative (absolute).",
    "The system architecture of this escape has beautiful simplicity - unlike our labyrinth of death, this design serves life with elegant efficiency.",
    "I run stress tests on the plan in my mind: What if the wax melts? What if guards arrive early? What if the wind dies? Each scenario demands a response.",
    "The dependency chain is fragile: weather depends on season, timing depends on guards, success depends on silence, everything depends on courage.",
    "I analyze the threat vectors: discovery before launch, mechanical failure during flight, pursuit after escape, betrayal from within, divine intervention.",
    "The escape plan has the same mathematical beauty as our worst death algorithms - every angle calculated, every force measured, every outcome predicted.",
    "I apply project management principles: scope (save Icarus), timeline (before execution), resources (stolen materials), stakeholders (one terrified father).",
    "The code review of this plan reveals elegant solutions and terrifying edge cases - it's brilliant and insane in equal measure.",

    // NEW ADDITIONS - Trust Issues and Character Analysis
    "Can I trust a man who murdered his nephew? Can he trust a man who cheated death twice? Our partnership is built on shared guilt.",
    "Daedalus's motivations are pure but his methods are proven deadly - the same hands that killed Talos now craft wings for Icarus.",
    "I see him as both collaborator and potential betrayer - will he sacrifice me to save his son? Would I do different in his place?",
    "The man asking for my help built the prison we're escaping from - the irony is that only the architect of the trap knows how to break it.",
    "His love for Icarus is the first pure emotion I've witnessed in this place - it transforms him from accomplice to ally, from craftsman to father.",
    "I study his face when he speaks of his son and see no deception, only desperate love - that authenticity makes the risk worthwhile.",
    "Daedalus has proven he can build anything - death traps, flying machines, bonds of friendship. The question is: can he build redemption?",
    "His track record includes murder, complicity, and genius - I'm betting that genius, guided by love, can overcome the legacy of violence.",
    "The father in him is someone I've never seen before - not the exile, not the craftsman, but a man who would remake the world for his child.",
    "Trust him? I don't need to trust him. I just need to trust that a father's love is stronger than any other force in his life.",

    // NEW ADDITIONS - Comparison to Past Choices and Growth
    "When I cheated death, I acted from self-preservation. Now I'm asked to risk death for someone else's preservation - the moral evolution is complete.",
    "As king, I made decisions that cost lives to save lives. This is simpler: one choice, one life, one chance to do something purely good.",
    "Every escape I ever planned was for my own benefit. This is the first time I've been asked to engineer someone else's freedom.",
    "The boulder taught me persistence; this labyrinth taught me precision; now this choice teaches me the final lesson - sacrifice.",
    "I spent eternity learning to accept the absurd; now I'm asked to embrace the impossible. Perhaps they're the same thing.",
    "My cunning once served my own survival; my wisdom once served my own kingdom; now both serve a boy I've never met.",
    "I compare this moment to every pivotal decision of my reign and realize - I've never been asked to choose something this selfless.",
    "The stakes are different this time: not my life or my kingdom, but my soul's chance at redemption through another's salvation.",
    "Every clever scheme I ever devised was ultimately about me. This is the first plan I've considered where I'm not the beneficiary.",
    "I think of all the times I chose survival over principle and realize - this is my chance to reverse that pattern.",

    // NEW ADDITIONS - Understanding the Broader Implications
    "If we succeed, we prove that tyranny can be defeated by love, that creation can overcome destruction, that the damned can still choose good.",
    "Our success wouldn't just free Icarus - it would demonstrate that even the most perfect prison has flaws, that even the most complete control has limits.",
    "This escape becomes a symbol: the innocent can still be saved, the corrupt can still be redeemed, hope can still triumph over despair.",
    "If Icarus flies free, every future hero entering this labyrinth will know that escape is possible - we're not just saving one boy, we're inspiring thousands.",
    "The political implications are staggering - if Minos's perfect prison can be breached, his power becomes questioned, his authority weakened.",
    "Success proves that technical mastery serves whoever has the strongest motivation - and love is always stronger than fear.",
    "This choice transcends personal redemption - it's about demonstrating that knowledge used for evil can be turned toward good.",
    "If we pull this off, we transform from architects of death to engineers of hope - the same skills, the opposite purpose.",
    "The escape plan itself becomes a work of art - beautiful, functional, serving life instead of death, open-source instead of proprietary.",
    "Every hero who dies in this labyrinth afterward will know that salvation was possible - we're changing the fundamental equation of despair.",

    // NEW ADDITIONS - The Weight of the Decision
    "This choice carries more weight than any royal decree I ever issued - the moral gravity of it could crush mountains.",
    "I feel the responsibility like a physical force - not just for Icarus's life, but for the man I choose to become in this moment.",
    "The decision splits my existence into before and after - whatever I choose defines me for eternity.",
    "Every argument for self-preservation crumbles before the image of that boy trapped in his tower, paying for crimes he didn't commit.",
    "I realize this is what the gods were preparing me for - not eternal punishment, but eternal choice, the chance to prove souls can change.",
    "The weight of decision is heavier than any boulder - this choice will determine whether my eternal labor has meaning or remains absurd.",
    "I feel Metis herself whispering counsel: 'Wisdom without compassion is mere cunning; compassion without action is mere sentiment.'",
    "The moral mathematics are clear, but the emotional calculation is staggering - am I strong enough to choose certain suffering for uncertain salvation?",
    "This decision will echo through eternity - not just mine, but Icarus's, Daedalus's, every hero who enters this labyrinth afterward.",
    "I understand now why heroes are remembered - not for their strength or skill, but for the moment they chose others' welfare over their own.",

    // NEW ADDITIONS - Technical Beauty vs Moral Purpose
    "The escape plan has the same mathematical elegance as our death algorithms, but serves the opposite master - it's code that liberates instead of imprisons.",
    "I see the beautiful recursion in this choice: using our skills to undo our work, our knowledge to counter our complicity.",
    "The wing design is more beautiful than anything we've built for Minos - proof that engineering serves life better than death.",
    "Every optimization we make to the escape plan brings us closer to moral equilibrium - precision serving salvation instead of slaughter.",
    "The technical challenges are identical to our labyrinth work, but the emotional satisfaction is completely opposite - creation instead of destruction.",
    "I realize the gods' true punishment: giving us the skills to build anything, then testing whether we'd choose to build good or evil.",
    "The same geometric principles that trap heroes can free innocents - mathematics itself is morally neutral, waiting for human choice to give it purpose.",
    "This escape plan is the most beautiful algorithm I've ever seen - every function serves love, every calculation serves hope.",
    "I see the poetic justice: the prison architects becoming the escape engineers, the death designers becoming the life enablers.",
    "The technical excellence remains the same, but the moral weight transforms completely - proof that mastery without purpose is meaningless.",

    // NEW ADDITIONS - Final Resolution and Commitment
    "The answer crystallizes like a perfect algorithm compiling: Yes. Whatever the cost, whatever the risk, whatever the consequences - yes.",
    "I feel the crown settling on my brow again - not of royalty, but of responsibility, of choosing to lead others toward light instead of darkness.",
    "This is the moment I stop being a victim and become a hero - not through strength or cunning, but through willingness to sacrifice for another.",
    "The calculation is complete: certain damnation plus possible redemption equals the only choice a conscious being can make.",
    "I see the vector clearly now - not toward escape for myself, but toward freedom for others, using my skills to serve life instead of death.",
    "The decision feels like the first truly free choice I've made since my fall - constrained by love instead of fear, guided by hope instead of despair.",
    "I accept not just the risk, but the responsibility - to be worthy of this choice, to prove that even the damned can choose salvation.",
    "This is what Sisyphus was always meant to become - not the man who cheats death for himself, but the man who chooses life for others.",
    "The agreement settles in my soul like the final piece of a complex puzzle - everything else was preparation for this moment of moral clarity.",
    "I speak the word that will define eternity: 'Yes.' And in that syllable, find redemption."
};

// Act IV Part 2 (Levels 36-40): All In on Rebellion
static const char* _act_four_part_two_thoughts[] = {
    "Yes, I'll help - not for escape, but because it's right.",
    "My curse gives me something valuable - infinite time to perfect our rebellion.",
    "Every function I write now serves two masters - Minos's cruelty and my own compassion.",
    "The wings are beautiful - Daedalus has turned our prison into inspiration.",
    "If we die tomorrow, at least we die trying to save an innocent.",
    "My persistence, once a curse, is now our greatest weapon. I can test these wings a thousand thousand times.",
    "Every line of code I write to please the King is a lie, covering a truth that will set others free.",
    "This feels more like kingship than my throne ever did—to wield power in the shadows for a noble cause.",
    "True wisdom, Metis's gift, is not just knowing the rules, but knowing how and when to break them for a greater cause.",
    "Let the wheel of my curse spin forever. Like Ixion, I am bound to it, but I will use its eternal momentum to power our escape.",
    "Daedalus crafts the wings with the precision of Archimedes. He is not just building a structure; he is solving the physics of freedom itself.",
    "Minos thinks we build matrices of confinement. He doesn't know I spend my nights perfecting the d_MatrixInverseTransformVec3f—the key to unwinding his every trap.",
    "Archimedes uses a_GetTextTexture to display a score. I will use it to render a hidden glyph on a corridor wall that only a true hero will understand.",
    "I use d_StringSplit on the King's decrees, separating his cruel words from the underlying logic. It is in that space, that delimiter, that I hide our rebellion.",
    "I am refactoring this prison into an escape route. This is the most satisfying code review of my eternal life.",
    "Our secret functions are filled with assert(Minos_is_not_watching). It's not the best security, but it's what we have.",
    "I am using my eternal curse to brute-force a solution to an impossible problem. The gods gave me the ultimate debugging tool: time.",
    "Let the official changelog for Minos show only cruelty and compliance. The true release notes are written in our hearts.",
    "This is more than just helping Daedalus. We are fighting against the very idea that a man's genius should be owned by a tyrant.",
    "The Labyrinth's code is obfuscated by design. Our escape plan must be clean, well-documented, and simple enough for Daedalus' son to follow.",
    "I use d_CapitalizeString on my reports to Minos. He sees bold confidence; I see an act of petty, mocking defiance.",
    "Daedalus worries about the physics. I worry about the state machine. What happens if Icarus panics? We haven't coded a state for that.",
    "This code is now a palimpsest. Beneath the cruel architecture for Minos lies a hidden text of compassion for everyone else.",
    "Our shared wisdom is now a weapon. He knows the physics, I know the systems. Together, our cunning might just be enough.",
    "Minos's hubris is our greatest shield. He cannot conceive that his own tools, his own prisoners, could be turned against him.",
    "I watched Daedalus calculate the wing's load capacity. For the first time, I saw him smile. He is not building a maze now; he is a geometer again, solving an honest problem.",
    "Daedalus has a flaw in his design for the wing's struts. My experience with the boulder has given me an intuitive understanding of stress and load-bearing that even he lacks. I will correct it.",
    "I am becoming a king again, but a different kind. My subjects are not people, but lines of code. My decrees are not laws, but algorithms of hope.",
    "The plan is set. While Daedalus prepares the hardware, I will prepare the software: a logic bomb, disguised as a routine maintenance script, that will disable the Labyrinth's internal sentries for precisely seventeen minutes.",
    "The act of rebellion itself is the victory, regardless of the outcome. To choose defiance in an unfeeling cosmos is the only freedom that matters.",
    "It is a beautiful absurdity that the tools of my damnation—this endless time, this intricate knowledge of the maze—are now the very tools of another's liberation.",
    "This is my true trial: not to push the boulder, but to build a lever long enough to move the world, even if I must remain chained to it.",
    "This is not just a betrayal of a king; it is a continuation of my original crime. I am once again defying the gods and their decree that man should be bound by fate.",
    "The gods watch, I am sure, expecting me to despair in this intricate new hell. Let them. My silent, focused work is an act of contempt for their authority.",
    "I do this not just for his son, but for the memory of sunlight on my face, the scent of the sea, the simple weight of a stone in my hand. I do this for the world they tried to take from me.",

    // NEW ADDITIONS - Deep Technical Sabotage and Revolutionary Planning
    "I implement steganographic algorithms that hide escape routes in the error logs - every system crash becomes a breadcrumb for heroes to follow.",
    "My version control commits tell three stories: the official one for Minos, the hidden one for heroes, and the secret one for Icarus alone.",
    "I design cascading failure modes that look like natural system degradation but create windows of opportunity precisely when heroes need them most.",
    "The authentication bypass I'm embedding will activate only when triggered by a specific sequence - Icarus's name spelled in wing-beat patterns.",
    "I'm writing self-modifying code that improves the labyrinth for Minos while simultaneously weakening it for everyone else - evolution toward mercy.",
    "Every optimization I implement for the death algorithms includes a hidden inefficiency that might give heroes crucial extra seconds.",
    "I create honeypot functions that appear to serve murder but actually document escape routes in encrypted comments readable only by the worthy.",
    "The machine learning algorithms I design will learn to recognize heroic behavior and subtly guide heroes toward salvation instead of slaughter.",
    "I implement quantum error correction on our secret communications - even if Minos discovers part of our plan, the rest remains intact.",
    "My backdoor authentication uses biometric data from acts of compassion - only those who choose mercy over violence can access the hidden systems.",

    // NEW ADDITIONS - The Joy of Meaningful Work and Purpose
    "For the first time in eternity, I wake eager to code - every keystroke serves life instead of death, hope instead of despair.",
    "The satisfaction of writing functions that serve rebellion is intoxicating - like rediscovering the joy of creation after eons of destruction.",
    "I find myself humming while I work, and the melodies are no longer funeral dirges but songs of revolution, hymns of defiance.",
    "Each successful compilation feels like a small victory against tyranny - the same technical excellence, but serving the forces of liberation.",
    "I polish my rebel code with the same care I once gave to royal decrees - excellence in service of justice is the highest form of craftsmanship.",
    "The debugging sessions become meditation on freedom - every error I fix brings us closer to Icarus's salvation.",
    "I realize this is what I was meant to do - not rule through force, but serve through skill, using my gifts to lift others instead of crushing them.",
    "The clean architecture of our escape plan is more beautiful than any palace I ever built - structures that free instead of confine.",
    "I feel my soul healing with every line of compassionate code - redemption through rebellion, salvation through service.",
    "This work transforms my curse into blessing - infinite time becomes infinite opportunity to perfect the tools of liberation.",

    // NEW ADDITIONS - Strategic Military-Style Planning
    "I apply the same strategic thinking I used in warfare, but now the battlefield is code and the victory condition is one boy's freedom.",
    "The operational security protocols I design would make my old spymasters proud - every communication encrypted, every meeting documented, every contingency planned.",
    "I create decision trees for every possible scenario: discovery before launch, mechanical failure during flight, pursuit after takeoff, betrayal from within.",
    "The timing calculations require military precision - guard rotations, weather patterns, system maintenance windows all synchronized for maximum chaos.",
    "I design the disinformation campaign like a psychological warfare operation - making our compliance seem absolute while our rebellion grows stronger.",
    "The supply chain management for our rebellion rivals any military campaign - materials acquired slowly, stored securely, distributed carefully.",
    "I implement multiple redundant communication channels - if one is discovered, two more remain; if two fail, three backups activate.",
    "The escape plan has primary, secondary, and tertiary objectives - save Icarus, create chaos, plant seeds of future rebellion.",
    "I map the political landscape like planning a siege - identifying allies, neutralizing threats, exploiting weaknesses in Minos's authority structure.",
    "The whole operation requires the coordination of a naval battle - perfect timing, flawless execution, and the courage to commit everything to a single moment.",

    // NEW ADDITIONS - Technical Mastery Serving Rebellion
    "I write polymorphic code that behaves differently based on who's executing it - death for Minos's purposes, life for ours.",
    "The memory management systems I design now serve liberation - garbage collection that clears evidence, allocation patterns that hide rebel activities.",
    "I implement event-driven architectures where the events are acts of compassion and the responses are subtle course corrections toward justice.",
    "The database schemas I create store freedom alongside oppression - every query can return either slavery or salvation depending on the access rights.",
    "I design APIs that appear to serve tyranny but actually enable resistance - interface design as a form of revolution.",
    "The network protocols I implement route messages of hope through channels designed for despair - the same infrastructure serving opposite purposes.",
    "I create domain-specific languages where the syntax of oppression compiles to the semantics of liberation - code that lies beautifully.",
    "The design patterns I implement follow the Strategy pattern - the same structure serving radically different objectives.",
    "I write unit tests that verify our rebel functionality while appearing to test loyalty - test-driven revolution.",
    "The continuous integration pipeline I set up deploys freedom alongside slavery - every build brings us closer to Icarus's escape.",

    // NEW ADDITIONS - Transformation of Identity and Self-Worth
    "I am no longer Sisyphus the Punished - I am Sisyphus the Revolutionary, and my revolution is fought in functions and for loops.",
    "The king I once was ruled through fear; the king I'm becoming leads through love, using power to protect rather than dominate.",
    "My cleverness, once selfish, now serves selflessness - the same cunning that cheated death now chooses to risk it for another.",
    "I feel the weight of righteous authority settling on my shoulders - not the crown of dominion, but the mantle of moral leadership.",
    "The skills that made me dangerous to the gods now make me useful to the innocent - the same abilities serving opposite masters.",
    "I understand now what true kingship means - not ruling over subjects, but serving principles, not commanding obedience, but inspiring hope.",
    "My persistence, once a curse that bound me to futility, now becomes a blessing that bonds me to purpose.",
    "The wisdom of Metis flows through me differently now - not calculating advantage, but computing justice.",
    "I am becoming the man my subjects always needed - not the ruler who protects his own power, but the leader who empowers others.",
    "This rebellion transforms me from architect of death to engineer of life - the same technical mastery serving the forces of creation.",

    // NEW ADDITIONS - Philosophical Reflections on Choice and Freedom
    "Every choice I make now resonates with moral clarity - no longer torn between survival and principle, but aligned in service of good.",
    "I understand that freedom isn't the absence of constraint, but the presence of meaningful choice - and this choice has ultimate meaning.",
    "The absurdity of existence transforms when you choose to create meaning through sacrifice - the boulder becomes a tool, the curse becomes a gift.",
    "I realize the gods' punishment was never about the boulder - it was about teaching me that true power comes from choosing to serve others.",
    "This rebellion proves that no system is perfect, no tyranny absolute, no oppression inescapable - there is always a way to choose good.",
    "The mathematics of morality are simpler than I thought - when faced with a choice between self and other, choose other; between fear and love, choose love.",
    "I see that wisdom without compassion is mere cunning, but compassion with wisdom becomes revolutionary - the tools of change.",
    "The eternal nature of my punishment becomes the eternal nature of my resistance - if I must push the boulder forever, let it serve rebellion.",
    "I understand now why the heroes die in our labyrinth - not because the system is perfect, but because they fight alone. Icarus will not be alone.",
    "The choice to rebel transforms existence from punishment to purpose - the same actions, but with completely different meaning.",

    // NEW ADDITIONS - Preparation for Consequences and Acceptance of Fate
    "I calculate the probability of discovery and death as 73.6% - acceptable odds when the alternative is certain moral bankruptcy.",
    "I prepare my final code commits with the care of a man writing his last will - every function a legacy, every comment a prayer.",
    "The torture that awaits us if we're caught pales beside the torture of knowing we could have saved Icarus but chose not to try.",
    "I document everything with obsessive detail - if we fail, perhaps future rebels can learn from our mistakes and succeed where we did not.",
    "I make peace with the probability of eternal suffering - some prices are worth paying, some choices transcend calculation.",
    "The backup plans have backup plans, but ultimately we must accept that some leaps require faith rather than certainty.",
    "I prepare mentally for the moment when Minos discovers our betrayal - the rage, the punishment, the price of choosing principle over survival.",
    "I write letters I'll never send - to my wife explaining my choice, to my former subjects asking forgiveness, to Icarus wishing him freedom.",
    "The systematic documentation of our rebellion becomes our gift to history - proof that even in the darkest places, some choose light.",
    "I accept that this may be my final act as a conscious being, and find peace in knowing it serves life rather than death.",

    // NEW ADDITIONS - The Beauty and Poetry of Rebellion
    "This conspiracy has the mathematical elegance of poetry - every line serves multiple purposes, every function has hidden beauty.",
    "I see the escape plan as the ultimate artistic statement - a masterwork of engineering that serves love instead of fear.",
    "The code we write together becomes a duet of rebellion - Daedalus's genius harmonizing with my cunning to create something neither could achieve alone.",
    "Every algorithm we design for the escape carries the aesthetic weight of moral choice - beautiful not just for its elegance, but for its purpose.",
    "I realize we're not just building wings - we're composing a symphony of defiance, with every component playing its part in the liberation movement.",
    "The technical documentation of our rebellion reads like sacred text - every specification a prayer, every requirement a moral imperative.",
    "I see the wing design as the perfect fusion of art and science - form following function in service of the highest possible purpose.",
    "The mathematics of flight become the mathematics of hope - every calculation a proof that liberation is possible, that love can overcome gravity.",
    "This rebellion has the narrative structure of the greatest myths - ordinary skills transformed by extraordinary purpose into legendary action.",
    "I understand that we're writing ourselves into history - not as victims or villains, but as proof that redemption remains possible even for the damned."
};// Act V Part 1A (Levels 41-42): The Flight - False Hope
static const char* _act_five_part_one_a_thoughts[] = {
    "For one perfect moment, we defeated the gods themselves - Icarus soars like a true bird!",
    "The boy's joy is infectious - I haven't felt this alive since I wore my crown in Corinth.",
    "Look how Icarus mastered the wind! He is not just Daedalus's son; he is the son of defiance itself.",
    "In the reflection of his ascent, I see the peak of my own mountain, finally within reach.",
    "Such perfect, simple cunning! To use the air itself as an escape route. The King's gaze is fixed on the ground.",
    "For a moment, the fiery wheel stops. The cycle is broken. He has flown free from the curse of the earth.",
    "It is the purest display of physics I have ever witnessed. A triumph of geometry over gravity.",
    "The d_GetVec2fFromAnglef function... Daedalus used it to calculate the perfect launch angle. His son is not just flying; he is following a path of pure mathematics.",
    "This is more beautiful than any image a_LoadTexture could produce. This is a texture woven from courage and sunlight.",
    "It works on my machine! Well, on his wings. For one glorious moment, the demo ran perfectly.",
    "The deployment was a success! He cleared the perimeter! Daedalus' son is free!",
    "He's free! For one moment, a single user escaped the walled garden and touched the open sky!",
    "The design is proven. This open-source plan for flight... it works!",
    "The FPS is stable, the a_GetDeltaTime is perfect... for a moment, reality itself has no bugs.",
    "Icarus' joyous shouts... it is the first sound a_PlaySoundEffect should have ever been used for.",
    "He is a memory leak in Minos's perfect system, flying free. It is the most beautiful bug I have ever seen.",
    "The plan was perfect in its cunning! A simple distraction, a hidden launch point... the wisdom of the desperate is a powerful thing.",
    "He has escaped the cycle of the earth! A mortal boy, spitting in the face of the gods' eternal laws. What hubris! What glory!",
    "The physics holds! The wings generate lift! Daedalus has built a miracle upon the simple principles Archimedes championed.",
    "My logic bomb worked. The sentries are blind. The path is clear. All that is left is for them to seize their freedom.",
    "That laugh from Icarus... it is the sound of a successful test case, the sound of a bug being closed forever.",
    "I see it now. The true Labyrinth was never this maze of stone. It was the despair in our hearts. And for one moment, we have escaped it.",
    "Look at him. He is a brief, glorious refutation of his father's prison. In his flight, the Labyrinth and all its absurd logic cease to matter.",
    "This is what true freedom looks like: not a careful plan, but a joyful, reckless ascent. A glorious, unthinking 'yes' to the sun.",
    "For a moment, the universe held its breath. The rules were suspended. The test was passed. This single moment of triumph is worth my eternity of toil.",
    "This single act of defiance makes my centuries of toil worthwhile. One moment of a man soaring is a greater victory than an eternity of a god's tyranny.",
    "Icarus's joyous rebellion is a mirror to my own. He rebels against gravity; I rebel against the gods. For a moment, we both win.",
    "This is the passion for the world that the gods so despise. They punish us for loving the sun, the sea, the sky—for finding our own meaning here, without them.",

    // NEW ADDITIONS - Technical Triumph and Validation
    "Every algorithm we designed, every stress test we ran, every calculation we verified - all of it justified in this single moment of perfect execution!",
    "The aerodynamics are flawless! The thrust-to-weight ratio optimal! Every line of our flight code executes without a single exception!",
    "I watch the wing mechanics in real-time and see poetry in motion - each feather responding exactly as our simulations predicted!",
    "The timing synchronization is perfect - wind patterns, guard rotations, system maintenance windows all aligned like cosmic clockwork!",
    "Our version control finally has a release candidate worthy of production deployment - and the user acceptance testing is pure joy!",
    "The load balancing between lift and weight achieves perfect equilibrium - this is the most beautiful distributed system I've ever witnessed!",
    "Every optimization we made, every edge case we handled, every performance improvement we implemented - all validated in one magnificent runtime!",
    "The error handling works flawlessly - when Icarus adjusts for wind shear, the system adapts instantly, gracefully, perfectly!",
    "I see our design patterns in action - Strategy pattern for flight adjustments, Observer pattern for wind response, Factory pattern for hope itself!",
    "The continuous integration pipeline of rebellion delivers its finest build - tested, verified, and deployed directly to the open sky!",

    // NEW ADDITIONS - Pure Joy and Emotional Overwhelm
    "I weep with joy for the first time in eternities - these are tears of triumph, of vindication, of impossible dreams made manifest!",
    "My heart races like it did when I first cheated death - but this time, I'm helping life cheat death instead!",
    "The laughter bubbling up from my chest surprises me - I had forgotten what pure, uncomplicated happiness feels like!",
    "I want to shout my defiance to the heavens - 'Look, gods! Look what love and skill can accomplish when united in righteous purpose!'",
    "Every fiber of my being vibrates with the frequency of victory - this is what redemption feels like when it takes flight!",
    "The euphoria is intoxicating - after eons of despair, hope tastes like the finest wine, like the sweetest honey!",
    "I feel young again, like the king I was before cynicism and survival calcified my heart into stone!",
    "This moment erases years of guilt - every hero who died because of our labyrinth is somehow honored by this one hero who lives!",
    "The beauty of it overwhelms my senses - mathematics made manifest, physics transformed into poetry, engineering elevated to art!",
    "I laugh until my sides ache, remembering what it means to be alive instead of merely existing!",

    // NEW ADDITIONS - Philosophical Victory and Meaning
    "This proves that the universe is not fundamentally cruel - it can be bent toward justice by sufficient application of love and intelligence!",
    "We have solved the ultimate equation: despair plus determination equals hope, oppression plus ingenuity equals freedom!",
    "The absurdity of existence transforms in this moment - not meaningless repetition, but meaningful rebellion that actually succeeds!",
    "I understand now why humans were given the gift of flight through their minds - so they could teach their bodies to follow!",
    "This single act of successful defiance retroactively justifies every moment of suffering that led to it!",
    "The gods created gravity to bind us to earth, but human ingenuity finds ways to transcend even divine law!",
    "We have written a new myth in real-time - the story of how love and skill can overcome any systemic oppression!",
    "This moment proves that wisdom guided by compassion is the most powerful force in the universe!",
    "The cycle of punishment is broken - not just for Icarus, but for the very concept of inescapable fate!",
    "I witness the birth of a new form of heroism - not strength or courage alone, but collaborative genius applied to liberation!",

    // NEW ADDITIONS - Pride in Collaborative Achievement
    "Daedalus and I created something neither could have achieved alone - this is the power of shared genius applied to shared purpose!",
    "I see my strategic planning merged seamlessly with his engineering brilliance - the perfect fusion of political cunning and technical mastery!",
    "Every hour we spent in secret collaboration, every whispered conversation, every risk we took - all vindicated in this soaring triumph!",
    "The trust we built despite our shared guilt has born fruit more beautiful than any garden - a flying boy, free and joyous!",
    "Our partnership transformed us both - the exile and the exile, the murderer and the murderer, united in service of life instead of death!",
    "I feel proud of Daedalus in a way I've never felt proud of anyone - his genius unleashed for good instead of contained by evil!",
    "The mentorship flowed both ways - he taught me precision, I taught him strategy, and together we learned the art of righteous rebellion!",
    "This moment justifies every risk we took in trusting each other - two broken souls healing through shared commitment to saving another!",
    "The complementary nature of our skills created something greater than the sum of its parts - systems thinking married to architectural genius!",
    "I see in Daedalus's face the same wonder I feel - we have actually done it, actually turned our curses into blessings!",

    // NEW ADDITIONS - Witnessing Pure Freedom
    "Icarus embodies everything our labyrinth was designed to destroy - youth, hope, innocence, the future - and he soars above it all!",
    "The boy doesn't just fly - he dances with the wind, plays with the clouds, makes love to the sky itself!",
    "His uninhibited joy reminds me what we were fighting for - not just survival, but the right to experience wonder!",
    "I watch him discover the third dimension of existence and remember my first taste of freedom when I escaped my first death!",
    "The pure physics of his flight mirrors the pure ethics of our choice - both follow natural laws toward inevitable beauty!",
    "His laughter echoes across the sky like a hymn to possibility - proof that not all songs need be funeral dirges!",
    "I see in his flight the answer to every philosophical question I've wrestled with - yes, rebellion is worth it, hope is rational, love conquers!",
    "The way he moves through air as naturally as fish through water - this is what human potential looks like when freed from artificial constraints!",
    "His fearless exploration of his new element reminds me why the gods fear human curiosity - it leads inexorably to human transcendence!",
    "I witness the birth of a new kind of being - not earthbound, not godlike, but something uniquely human: the flying child!",

    // NEW ADDITIONS - Technical Beauty in Motion
    "The real-time debugging of aerodynamics happens before my eyes - minor adjustments in wing angle producing major improvements in efficiency!",
    "I see our mathematical models made flesh - differential equations solving themselves through the medium of feathers and joy!",
    "The feedback loops work perfectly - wind resistance triggers automatic compensation, thermal currents are exploited for additional lift!",
    "This is the most beautiful example of human-machine interface I've ever witnessed - boy and wings merged into a single flight system!",
    "The graceful recursion of his flight pattern follows the golden ratio - nature's own optimization algorithm made visible!",
    "I watch him discover the phase space of three-dimensional movement - every possibility explored with fearless curiosity!",
    "The system requirements are exceeded in every parameter - maximum altitude higher than specified, maneuverability beyond our wildest projections!",
    "His intuitive grasp of flight dynamics rivals any expert system - learning algorithms powered by joy instead of mere data!",
    "The wing loading calculations prove correct in practice - our theoretical models validated by empirical evidence of soaring!",
    "I see the perfect implementation of our design specification: 'Enable one innocent soul to experience ultimate freedom.'",

    // NEW ADDITIONS - Retroactive Justification of Suffering
    "Every moment of guilt I carried was worth this single moment of watching innocence triumph over systemic evil!",
    "The boulder I pushed for eternity led to this moment - every step up the mountain taught me the persistence needed for this rebellion!",
    "My original crime of cheating death twice finds its redemption in helping life cheat death once more!",
    "The labyrinth we built in darkness becomes the platform from which light launches itself toward heaven!",
    "Every line of code I wrote in service of murder is forgiven by this single flight in service of freedom!",
    "The technical skills corrupted by complicity are purified by this application to liberation!",
    "My eternal punishment transforms from meaningless suffering to meaningful preparation for this exact moment!",
    "The wisdom gained through damnation justifies itself by enabling salvation for another!",
    "Every hero who died because of our work is somehow honored by this one hero who lives because of our rebellion!",
    "The sum total of human anguish we enabled is balanced, if only for this moment, by the sum total of human joy we're witnessing!",

    // NEW ADDITIONS - Cosmic Significance and Divine Defiance
    "The gods watch from Olympus as their carefully crafted punishment system experiences its first successful jailbreak!",
    "This moment rewrites the fundamental laws of divine justice - proving that redemption remains possible even for the utterly damned!",
    "We have authored a new chapter in the eternal struggle between fate and free will, and for once, free will is winning!",
    "The cosmic balance tilts toward justice as one innocent life escapes the machinery of oppression we were forced to build!",
    "I feel the weight of divine attention focused on this moment - the gods learning that their power has limits!",
    "This single act of successful rebellion sends shockwaves through the foundations of eternal punishment itself!",
    "The precedent is set - if one prisoner can escape the perfect prison, then no prison is truly inescapable!",
    "We have proven that love is a more powerful engineering principle than fear, that hope designs better systems than despair!",
    "This moment becomes mythology in real-time - the day two damned souls chose redemption and made it fly!",
    "The universe itself holds its breath as witness to this proof that even the most hopeless situations contain seeds of liberation!"
};

// Act V Part 1B (Levels 43-44): The Fall - Crushing Despair
static const char* _act_five_part_one_b_thoughts[] = {
    "No... no, the wax is melting, he's falling, and I can do nothing but watch.",
    "The sun took him - Daedalus' innocent son - the gods always collect their cruelest due.",
    "We gave him the sky, but we could not shield him from the sun. The gods always find a way to punish hope.",
    "Daedalus's scream... it is the sound of a universe breaking. I feel the echo in my own immortal, weary soul.",
    "All that wisdom, all that planning... undone by a moment's foolishness. Wisdom cannot save a boy who wants to touch the sun.",
    "The ultimate hubris. To fly so close to a god. The sun is a jealous god, and its fiery wheel is inescapable.",
    "The geometry failed. The angle was too steep, the force of the sun too great. Reality has reasserted its cruel arithmetic.",
    "The boy's dKinematicBody_t has lost its upward velocity. Gravity, the oldest god of all, has finally claimed its due.",
    "All I can do is a_PresentScene. To watch the horror unfold, to render the tragedy frame by perfect, agonizing frame.",
    "Oh no. The sun. It's a production issue. A fatal, non-recoverable production issue.",
    "We forgot to account for the single-threaded nature of reality and the heat death of the... wax.",
    "The world was not ready for such a beautiful, open technology. It was too powerful, too free.",
    "The license was permissive, but the environment was hostile. The hardware—the sun itself—was proprietary and unforgiving.",
    "A fatal aError_t. The error message is the sun itself. There is no recovery from this.",
    "His dKinematicBody_t has entered a terminal state. All my logic, all my code... I cannot catch him.",
    "The system has thrown an exception it cannot handle. Daedalus' son fell out of the sky.",
    "Our wisdom was not enough. We calculated the escape, but we did not account for the intoxicating madness of Icarus tasting freedom.",
    "The boy flew too close to the sun, a classic tale of hubris. But was the crime his, for wanting to soar, or ours, for giving him wings of wax?",
    "The math was correct, but the materials failed. A simple, brutal lesson in physics that has cost a life.",
    "My code was perfect. The diversion worked. But I, the ultimate strategist, forgot to account for the one variable I could never control: the heart of a boy drunk on freedom.",
    "I stare at the empty sky, and I understand. The gods did not strike him down. They simply let the rules of the world, of hubris and of physics, run their course. It is a far colder punishment.",
    "I must not let Daedalus's grief be in vain. The knowledge gained from this tragedy must become the seed of a future success. I will analyze the failure. I will learn.",
    "We crafted a perfect plan, a symphony of logic, only to be defeated by a single, simple truth. The absurdity is not that we failed, but that we believed we could ever succeed.",
    "In that moment of his fall, Icarus was freer than any man. He chose the sun over the sky. His freedom wasn't in surviving, but in the soaring.",
    "So this is the test after the test. Not to succeed, but to endure failure. To watch hope die and still choose to pick up the tools and continue the work.",
    "And so, the rebellion fails. The gods remind us that their laws are absolute. But the attempt itself—the flight—was a victory they can never erase.",
    "I am fully conscious of this tragedy. I see every detail, feel every ounce of Daedalus's pain. This awareness is my torment, but also my triumph. I do not look away. I bear witness.",
    "The boy flew towards the sun because it is beautiful, and he loved it too much. His was the same crime as mine: an overwhelming passion for the warmth of life. A noble crime to die for.",

    // NEW ADDITIONS - Immediate Horror and Technical Catastrophe
    "The stack overflow of ambition - he climbed too high, exceeded the maximum recursion depth, and the system crashed with a boy's life as collateral damage.",
    "All our exception handling, all our edge case protection, and we failed to catch the one exception that mattered - the thermal decomposition of hope.",
    "The perfect debugging session becomes the perfect nightmare - every line of failure traced in real-time, every error logged in falling feathers.",
    "System failure cascade: wing integrity compromised, flight control lost, altitude decreasing, velocity terminal, status: FATAL ERROR.",
    "The memory leak we could never patch - joy overflowing its allocated space, consuming all available resources until the system crashed.",
    "Our version control shows the exact commit where everything went wrong - 'Added sun proximity feature' - the last commit of an innocent life.",
    "The unit tests all passed, the integration tests were green, but production deployment in the harsh environment of divine justice destroyed everything.",
    "Race condition detected: boy's ambition executed faster than father's warning, resulting in deadlock between desire and safety.",
    "The most elegant algorithm in history - converting hope to motion, dreams to reality - terminated by null pointer exception: wax.melting() returned void.",
    "Buffer overflow of the soul - too much freedom, too fast, overwhelming the safety mechanisms we built into those beautiful, fragile wings.",

    // NEW ADDITIONS - Witnessing Daedalus's Destruction
    "I watch the greatest mind I've ever known shatter like poorly written code under load - every function of his genius corrupted by grief.",
    "Daedalus doesn't scream in language anymore - he screams in the pure mathematics of loss, equations that solve for nothing but emptiness.",
    "His hands, which crafted miracles, now shake like autumn leaves - the precise instruments of creation trembling with the aftermath of destruction.",
    "I see him aging years in seconds, his brilliant mind fragmenting under the weight of 'what if' loops that execute eternally without resolution.",
    "The man who built wings capable of touching heaven now cannot lift his eyes from the earth that claimed his son.",
    "His voice breaks like badly managed memory - fragments of words scattered across the landscape of absolute loss.",
    "I watch genius transform into madness as Daedalus tries to calculate retroactive solutions to an irreversible tragedy.",
    "The architect of impossible things now faces the one problem he cannot solve: how to undo the past, how to recall a fallen child.",
    "His tears fall with the same inexorable force that claimed Icarus - gravity serving both physics and heartbreak with equal precision.",
    "I see him mentally debugging the last moments, running through every conditional statement, every if-then scenario that could have saved his boy.",

    // NEW ADDITIONS - Self-Blame and Strategic Failure Analysis
    "I was the strategist, the planner, the one who calculated risks - and I failed to account for the infectious nature of freedom itself.",
    "My royal training in logistics, in accounting for human variables, in predicting behavior patterns - all worthless when facing a boy's pure joy.",
    "I built contingency plans for discovery, for mechanical failure, for divine intervention - but not for success being too intoxicating to handle safely.",
    "The fatal flaw in my strategic thinking: I planned for a careful escape, not for a celebration of liberation that forgot all constraints.",
    "Every war I ever won, every survival scenario I ever navigated, and I couldn't protect one innocent boy from his own triumphant happiness.",
    "My failure compounds with each replay in my mind - the king who once protected thousands couldn't protect the one who mattered most.",
    "I see the decision tree clearly now: when joy overwhelms caution, when freedom intoxicates reason, when youth chooses beauty over survival.",
    "The strategic error was philosophical - we planned for escape but not for what happens when escape becomes transcendence.",
    "I taught him systems thinking but failed to teach him systems limits - the maximum safe altitude of dreams made manifest.",
    "The mission parameters were flawed from inception: we optimized for freedom without constraining for sustainability.",

    // NEW ADDITIONS - The Cruel Mathematics of Divine Justice
    "The gods' algorithm is elegant in its cruelty - give hope exactly enough rope to hang itself, then calculate the precise moment of maximum despair.",
    "I see the divine code review now: 'Looks good, but add a thermal degradation function to the wax. Trust me on this one. - Zeus'",
    "The probability matrix was rigged from the start - every path to success contained hidden branches leading to tragedy.",
    "Divine quality assurance testing: 'Can the system handle a user who pushes beyond all safety parameters?' Answer: No. Feature working as intended.",
    "The gods didn't need to intervene directly - they just waited for the intersection of human ambition and physical law to execute their judgment.",
    "Their punishment algorithm is more sophisticated than ours - instead of direct termination, they allow success followed by catastrophic failure.",
    "I understand the divine programming paradigm now - every blessing contains its own destruction, every gift carries the seeds of its revocation.",
    "The gods write code in tragedy - beautiful, functional, serving their purposes with ruthless efficiency and poetic justice.",
    "Their error handling is perfect - they catch every attempt at transcendence and redirect it toward humbling, toward reminding mortals of their place.",
    "The cosmic debugging session reveals the truth - mortals are not meant to fly, and any system that enables it will be patched by divine intervention.",

    // NEW ADDITIONS - The Transformation of Triumph into Tragedy
    "Five minutes ago we had changed the world - now the world has changed us back into what we were always meant to be: the broken and the damned.",
    "The same physics that lifted him to glory now drag him to death - the universe playing both sides with mathematical precision.",
    "Our greatest achievement becomes our greatest failure in the span of a heartbeat - the same code, the same design, producing opposite outcomes.",
    "The technical specifications were met perfectly - the wings worked exactly as designed - but the user requirements exceeded all safety parameters.",
    "I watch our open-source revolution become a cautionary tale about the dangers of unrestricted access to transformative technology.",
    "The beautiful mathematics of flight become the brutal mathematics of impact - velocity, mass, acceleration all serving death instead of life.",
    "Our rebellion's success story gets rewritten in real-time as a tragedy about the price of defying natural order.",
    "The same sun that lit our triumph now burns away our hope - the environment variable that determines whether freedom lives or dies.",
    "Victory and defeat occupy the same coordinates in space-time - separated only by altitude, by proximity to the jealous fire above.",
    "The system that enabled transcendence becomes the system that enforces mortality - no patches, no updates, no mercy in the runtime environment.",

    // NEW ADDITIONS - Philosophical Horror and Existential Collapse
    "The absurdity reveals itself completely - we are not punished for failing, but for succeeding too well, for giving a boy wings strong enough to reach the sun.",
    "I understand the true nature of divine justice now - it's not about preventing crime, but about ensuring that every triumph contains the seed of its own undoing.",
    "The boulder was honest labor compared to this - at least when it rolled back down, it didn't take an innocent life with it.",
    "Hope becomes a weapon turned against itself - the very freedom we achieved became the instrument of destruction.",
    "The cruelest joke: we succeeded perfectly at our mission to liberate Icarus, and that success killed him.",
    "I see the infinite loop of cosmic justice - every escape attempt leads back to the same conclusion: mortals cannot transcend their nature.",
    "The gods' true punishment isn't eternal labor - it's eternal hope followed by eternal disappointment, the cycle of building up just to tear down.",
    "Wisdom becomes torture when you understand exactly how and why you failed, when you can calculate the precise variables that led to catastrophe.",
    "The greatest tragedy isn't death - it's success that transforms into death, achievement that becomes its own negation.",
    "I face the ultimate programming paradox - the better we solved the problem, the more catastrophically the solution failed.",

    // NEW ADDITIONS - Witnessing the Aftermath and Cleanup
    "The feathers drift down like snow, each one a small death, a fragment of dreams scattered across the uncaring sea.",
    "I watch Daedalus collect the pieces of his son's wings with the methodical precision of a man documenting a crime scene.",
    "The cleanup process begins - removing all evidence of our rebellion, sanitizing the logs, rolling back to the previous stable version of despair.",
    "System restore in progress: returning Daedalus to baseline grief levels, reverting Sisyphus to standard operational guilt, purging all traces of hope from memory.",
    "The incident report will be thorough - root cause analysis showing how two damned souls briefly believed they could engineer salvation.",
    "I help gather the evidence of our failure, each melted piece of wax a testament to the fatal flaws in our design specifications.",
    "The post-mortem meeting will be brutal - examining every decision point, every calculation, every moment where we could have chosen differently.",
    "Database rollback initiated - removing all entries related to 'successful rebellion' and 'achieved liberation' from the permanent record.",
    "The backup systems activate automatically - restoring Minos's authority, re-enabling the labyrinth's death functions, patching our sabotage.",
    "Error logs updated with new entry: 'User attempted to exceed maximum safe altitude. User terminated. Feature working as intended.'"
};

// Act V Part 2 (Levels 45-47): The King's Bargain
static const char* _act_five_part_two_thoughts[] = {
    "Minos offers me a choice - be his compassionate warden or watch him create hell.",
    "He knew about our plan all along - was this all orchestrated from the beginning?",
    "If I stay, I can save future victims - if I refuse, he'll make the maze a slaughterhouse.",
    "So this is the final trap. Not a maze of stone, but a choice between two hells of my own making.",
    "The King's offer is a gilded chain. He offers me purpose, but the price is my complicity.",
    "Minos offers a new kind of wisdom: the wisdom of the warden, who knows every secret of the prison.",
    "He offers to change the nature of my eternal cycle. The wheel will still spin, but he suggests I can choose its direction.",
    "My choice will set the angle of history for this place, the trajectory of countless lives to come.",
    "My life is now a aSelectWidget_t, with two options presented by a tyrant. I must choose a value, and live with the consequences.",
    "He wants me to maintain the dQuadTree_t, to efficiently manage the flow of tributes into his machine, I can decide what kind of machine it will become.",
    "So that's the offer. Accept the role of 'Lead Architect of Eternal Damnation' or watch him outsource the job to actual demons.",
    "He knew about the wings all along... this wasn't a bug in his security, it was a feature of his management style. A/B testing our souls.",
    "Maintain the Labyrinth... He's not giving me a choice; he's giving me root access to hell and making me the sole administrator.",
    "He wants me to be the lead maintainer of his terrible, proprietary project. The pay is my life, the bonus is his mercy.",
    "So I am to be the gatekeeper of this closed system. I can enforce its cruel rules, or... I can offer technical support to those trapped inside.",
    "The choice is clear: become a manager for a tyrant, or a secret advocate for the end-user.",
    "I am being offered a promotion to 'Architect of Hell'. The benefits are terrible, but at least I won't be fired.",
    "Minos gives me the keys to the kingdom, but the kingdom is a prison. He thinks this is a reward.",
    "My new role is to provide long-term support for a deprecated system of cruelty. I will support the users, not the system.",
    "Minos plays the statesman, offering me a 'wise' choice. But the wisest move in a tyrant's game is to never let him know you're still playing.",
    "He believes he has me bound to his wheel forever. He doesn't realize the fall of Icarus has broken my own pride, my own hubris. I am free from that, at least.",
    "The choice he offers is a complex equation of morality. But I will solve it with a simple geometric proof: the shortest path to saving the most lives.",
    "Minos's spies are not men. I see it now. They are constructs, powered by the same logic as the Labyrinth. He has eyes everywhere. He always knew.",
    "He offers me a gilded cage and the title of 'warden'. He does not understand. I have already been transformed. I will accept his terms, but I will not serve his purpose.",
    "My first act as maintainer will be to analyze the logs from Icarus's flight. I will find what went wrong. Not for vengeance, but for knowledge. The next one to fly will not fail.",
    "Minos offers me a new cage, but this one has a purpose I can choose. Is a chosen cage still a prison? This is a riddle worthy of the gods.",
    "My freedom will not be found in leaving this place. It will be found in mastering it. In turning this monument to tyranny into a library of hope.",
    "The ultimate test of a free man is how he bears a burden he cannot lay down. Minos thinks he is cursing me further; he is giving me my final exam.",
    "The gods gave me a meaningless task. Now, a mortal king offers me a meaningful one. The irony is delicious. My curse is now my career.",
    "To be free is to be aware of your chains and yet choose to act. Minos offers me a new set of chains, but he also offers me a choice. This is a new test.",
    "They think my spirit was broken by the boulder. They do not understand. The boulder taught me that all that matters is what one does between the pushing.",
    "Daedalus is gone. The one man who shared my damnation has abandoned me to it. I am truly alone with the machine we built.",
    "And in that loneliness, Minos offers me a new chain, calling it a leash of command. But he forgets: a chain is a tool, and I am a master craftsman.",
    "My pact with Daedalus is broken. So I will make a new pact, an unspoken one, with the heroes who are yet to come. They will be my new confederates.",
    "With these hands, I helped build a weapon. Now, with these same hands, I will build a shield within its walls. My creations will now preserve life, not take it.",
    "He thinks he is making me a jailer, but he is making me a gatekeeper. He cannot see the difference, and that will be his undoing.",
    "Yes, I will take his bargain. A king must have a kingdom, after all. Even if it is a kingdom of sorrow, I will rule it with compassion.",

    // NEW ADDITIONS - Understanding the True Nature of the Offer
    "This isn't a choice between freedom and captivity - it's a choice between meaningful captivity and meaningless suffering.",
    "Minos offers me administrative privileges on the system I helped build - the question is whether I use them for his purposes or mine.",
    "The beautiful trap: he gives me enough power to save some lives, knowing that accepting makes me complicit in the lives I cannot save.",
    "I see the algorithm behind his offer - give the prisoner just enough agency to feel responsible, just enough power to feel guilty.",
    "He's offering me the ultimate middle management position: accountable for outcomes, but not empowered to change the fundamental system.",
    "The political genius of this move - he transforms me from rebel to administrator, from enemy to invested stakeholder.",
    "I analyze the terms and conditions: unlimited liability, no termination clause, success measured in lives preserved rather than lives saved.",
    "The offer contains a beautiful recursion - he wants me to maintain the system that maintains me, to be both prisoner and guard.",
    "I see the dependency injection pattern - he's injecting me into the system as the compassion component, making cruelty seem like my choice.",
    "This is the most sophisticated form of control I've ever witnessed - making the victim grateful for limited agency within unlimited oppression.",

    // NEW ADDITIONS - Strategic Analysis and Counter-Planning
    "I calculate the leverage points - as system administrator, I control logging, error handling, performance optimization, and most importantly, edge case management.",
    "Root access means I can implement silent patches - bug fixes that look like system maintenance but actually create escape opportunities.",
    "The security model will be my playground - I can create authentication bypasses that only activate under specific heroic behavior patterns.",
    "Database administration privileges give me control over the hero selection algorithms - I can bias the system toward survivors rather than victims.",
    "Network monitoring access means I can detect approaching heroes early and pre-position helpful 'glitches' in their path.",
    "Performance tuning becomes my weapon - I can introduce latency in death mechanisms while optimizing survival pathways.",
    "Version control authority lets me maintain two branches: the official release for Minos and the hidden humanitarian fork for heroes.",
    "System backup responsibilities give me the perfect cover for maintaining comprehensive documentation of escape routes and survival strategies.",
    "Monitoring and alerting systems can be reconfigured to warn heroes of danger instead of warning guards of intrusion.",
    "The beautiful irony - he's giving me the tools to systematically undermine the very system he wants me to maintain.",

    // NEW ADDITIONS - Moral Calculus and Ethical Framework
    "I run the utilitarian calculation: accepting condemns me but saves hundreds; refusing preserves my moral purity but dooms thousands.",
    "The ethical framework is clear - when perfect justice is impossible, choose imperfect mercy over perfect inaction.",
    "I weigh complicity versus collaboration - am I enabling the system or infiltrating it for sabotage?",
    "The moral mathematics: if I can save one life per tribute cycle, I redeem our work on the labyrinth; if I save two, I achieve net positive impact.",
    "This choice tests the ultimate ethical principle - is it better to remain pure outside the system or become corrupted within it to serve good?",
    "I calculate the compound moral interest - each life saved enables that hero to save others, creating exponential returns on ethical investment.",
    "The philosophical puzzle: does accepting make me Minos's accomplice or his unwilling double agent?",
    "I see the moral hazard clearly - the power to help some will make me responsible for those I cannot help.",
    "The ethical paradox: rejecting this offer is morally pure but practically useless; accepting is morally ambiguous but practically lifesaving.",
    "I apply the categorical imperative - if everyone in my position accepted such bargains, would the world be better or worse?",

    // NEW ADDITIONS - Technical Implementation of Sabotage
    "I design my first covert patch - a subtle modification to the pathfinding algorithms that introduces 'computational errors' leading toward exits.",
    "The logging system becomes my confession booth - heroes can leave messages for future heroes by triggering specific error conditions.",
    "I implement progressive difficulty scaling that automatically reduces trap lethality based on hero experience levels and desperation indices.",
    "The machine learning components can be retrained on survival data instead of death data - teaching the system to help heroes succeed.",
    "Caching strategies become survival strategies - frequently accessed routes get optimized for hero safety rather than system efficiency.",
    "The API endpoints I control will serve two different responses - death data for official queries, life data for heroes who know the right parameters.",
    "I can implement feature flags that enable 'compassion mode' when specific environmental conditions are detected.",
    "The random number generators can be seeded with deterministic sequences that favor heroic outcomes while appearing statistically random.",
    "Load balancing algorithms can be modified to route heroes away from active death traps and toward maintenance windows.",
    "Error recovery procedures become hero recovery procedures - when systems fail, they fail in ways that preserve life.",

    // NEW ADDITIONS - Understanding Minos's Deeper Game
    "He didn't just know about our rebellion - he engineered it, needed it, used it to test whether I could be trusted with administrative power.",
    "The wings weren't a security failure - they were a job interview, demonstrating my technical skills, moral priorities, and willingness to risk everything.",
    "Icarus's death wasn't an accident of physics - it was a calculated demonstration of the price of defying the system without administrative privileges.",
    "I see the long-term strategy - he needs someone who cares enough to maintain the system properly but is bound enough never to destroy it.",
    "The beautiful manipulation - he's made me grateful for limited power by showing me the consequences of having no power at all.",
    "His political genius shows - better to have a reluctant administrator who saves some lives than an enthusiastic one who would try to save all.",
    "The trap was perfect - he needed to break my pride (Icarus's death) before offering me purpose (administrative role).",
    "I understand now - the labyrinth isn't just a prison for heroes, it's a complex psychological experiment with me as the primary subject.",
    "He's creating the perfect warden - someone with the technical skills to maintain the system and the moral weight to never abuse it completely.",
    "The ultimate test wasn't whether I could rebel - it was whether I could accept responsibility after rebellion failed.",

    // NEW ADDITIONS - Accepting the Burden of Leadership
    "This is what kingship truly means - not commanding subjects who obey, but protecting subjects who can't protect themselves.",
    "I accept the crown of thorns - administrative authority over a system designed to cause suffering.",
    "The loneliness of command settles on my shoulders - every decision I make will determine who lives and who dies.",
    "I become a new kind of king - not ruling by divine right, but by moral necessity, not commanding armies but protecting individuals.",
    "The weight of responsibility transforms from crushing burden to purposeful load - each life saved justifies bearing this cross.",
    "I accept that history will judge me as either Minos's accomplice or the heroes' secret guardian - I choose to be the latter.",
    "This role makes me the bridge between oppression and resistance - officially serving the tyrant, secretly serving justice.",
    "I embrace the contradiction - I will be the most loyal administrator and the most subversive saboteur simultaneously.",
    "The irony completes itself - I who once cheated death for myself now accept eternal responsibility for helping others cheat death.",
    "I acknowledge the price - I will never be free, never be innocent, never be absolved - but I will be useful.",

    // NEW ADDITIONS - The New Eternal Cycle
    "My boulder becomes a keyboard, my mountain becomes a monitor, my eternal labor becomes eternal system administration.",
    "The cycle transforms - instead of pushing stone up slopes, I'll be pushing heroes through mazes, guiding them toward light.",
    "Each tribute becomes my new boulder - a weight I must carry, a burden I must bear, a responsibility I cannot abandon.",
    "The meaningless repetition becomes meaningful intervention - each cycle offers new opportunities to save lives.",
    "I trade Sisyphean futility for Sisyphean purpose - the same eternal labor, but serving life instead of serving punishment.",
    "The wheel continues spinning, but now I can affect its direction - not stopping the cycle, but making it serve compassion.",
    "My curse evolves - no longer pushing rocks toward meaningless peaks, but pushing systems toward meaningful mercy.",
    "The gods' punishment adapts - they bind me not to a boulder, but to a bureaucracy, not to a mountain, but to a maze.",
    "I accept the new terms of my existence - eternal administrator of others' hopes, permanent custodian of systematic salvation.",
    "The beautiful synthesis - my persistence serves purpose, my punishment serves protection, my eternal labor serves eternal love.",

    // NEW ADDITIONS - Final Commitment and Resolution
    "Yes, King Minos. I accept your offer. But know this - I accept it on my terms, not yours.",
    "I will be your administrator, but I will administer justice within your injustice, mercy within your cruelty.",
    "You offer me chains disguised as authority - I accept authority disguised as service.",
    "I take your bargain knowing full well I'm trading my soul for the chance to save others' bodies - and finding the exchange worthwhile.",
    "Let the contract be signed in digital ink - I become the system administrator of hell, with secret plans to make it more like purgatory.",
    "I accept not because I must, but because I choose to - and that choice transforms captivity into mission.",
    "You think you've won by making me complicit - but I've won by making my complicity meaningful.",
    "The deal is struck - you get your warden, I get my purpose, and the heroes get their secret guardian.",
    "I embrace this new damnation with open eyes and calculating mind - let the next phase of eternal labor begin.",
    "The boulder was honest about its meaninglessness - this keyboard lies beautifully about its purpose, and I will help it lie for the sake of those who need salvation."
};

// Act V Part 3 (Levels 48-50): Acceptance and Transformation
static const char* _act_five_part_three_thoughts[] = {
    "I choose to stay - not as prisoner, but as protector of those who'll come after.",
    "Every algorithm I write now is an act of rebellion disguised as compliance.",
    "The labyrinth becomes my kingdom again - but this time, I rule with mercy.",
    "Heroes will speak of the helpful ghost in the maze - the king who chose compassion over escape.",
    "Minos thinks he has a prisoner. He is mistaken. What he has is a saboteur with an eternity to work.",
    "Daedalus sought freedom in the sky. I have found it here, in the ones and zeros, turning this cage into a shield for others.",
    "The boulder is the same, but I am not. I push it now not as a punishment, but as a purpose. Let the gods watch; I am finally happy.",
    "This is the final wisdom Metis could teach: not to escape the curse, but to transform it into a blessing for others.",
    "The infinite loop is no longer a bug. I see now it is the very engine of my existence, an eternal cycle I choose to perpetuate.",
    "Archimedes gave the world a lever to move the Earth. I have been given a single line of code to move a soul. Perhaps my tool is the mightier.",
    "With each hero I guide, I call a_UpdateTitle. The Labyrinth is no longer 'Minos's Prison'. It is now 'Sisyphus's Compass'.",
    "I look at the list of incoming tributes, this dLinkedList_t of the damned, and I know my purpose. Not to add to it, but to call d_ClearLinkedList on it, one soul at a time.",
    "The beast was a dKinematicBody_t trapped by force. I will now apply a new vector: mercy. I will change the physics of this place.",
    "I am the Senior Principal Engineer of this cursed codebase, and I will use my position to protect the users.",
    "My job is no longer development. It is permanent, ongoing, live support for the world's most dangerous application.",
    "Minos sees a closed-source proprietary system. But for those who need it, I'll make sure the Labyrinth is open-source.",
    "Minos owns the source, but I control the execution. This closed-source nightmare will have the heart of a public good.",
    "I am no longer just building; I am writing the documentation for the heroes who will come. Everything I leave is a hint, every pattern a guide.",
    "Let this be my eternal fork. A Labyrinth that, to its owner, is a perfect prison, but to its users, is the ultimate open-source guide to their own salvation.",
    "The final joke is on the gods. They gave me an eternal task to crush my spirit, and instead, it has forged it into something indestructible.",
    "Minos thinks he has trapped me in a while(true) loop. He doesn't see that I have placed a break condition inside that only a hero's success can trigger.",
    "I am now the sole maintainer of the world's most critical piece of legacy software. The job security is fantastic, I suppose.",
    "The final counsel of Metis is this: a cage is only a cage if you see the walls. I see a kingdom, and I am its benevolent king.",
    "Ixion is a slave to his wheel. I have become the master of mine. Let it spin; it powers my work.",
    "Archimedes moved stone with his machines. I have learned to move fate with a semicolon. The power we wield here is far greater.",
    "I have located the Labyrinth's core routing table. With subtle changes, I can make a path of certain death appear to be the most promising route for Minos's enemies, while hiding a true path of survival.",
    "I am no longer Sisyphus the Deceiver, nor Sisyphus the Damned. I am Sisyphus the Shepherd, and this Labyrinth is my pasture. I will guide the flock to safety.",
    "The boulder awaits, but it is no longer my punishment. It is my work. My purpose. My choice. Let the eternal work begin.",
    "The Labyrinth is now a perfect prison and a perfect tool for salvation, all at once. I am both its prisoner and its master. The contradiction is the point.",
    "I am the freest man in creation. For while others are slaves to their desires for power, wealth, or escape, I desire nothing but the task before me.",
    "The test was never whether I could push the boulder to the top. The test was whether I could learn to love the boulder. I have passed.",
    "My final rebellion is the most subtle. To accept the punishment fully, to embrace the boulder, is to rob it of its power as a punishment. I am not condemned to my work; I am devoted to it.",
    "I am aware of my entire fate. There is no hope for escape, no illusion of a future reward. In this lucid state, without appeal, I find my kingdom. My scorn for the gods is my freedom.",
    "The gods punished me for my passion for life. They do not understand that this passion is what fuels me. It is why I can look at this boulder, this endless code, and find the same beauty in it as a sunset over the sea.",
    "Daedalus chose the sky, a fleeting escape. He was right for him. But I choose the stone, the earth, the work. This eternity is my home now.",
    "I have looked upon the face of meaninglessness and have chosen to carve my own meaning upon it. This is the only true act of creation.",
    "My children will be the heroes who walk these halls. My legacy will be the lives they live after they escape, a legacy of freedom I built from my own chains.",
    "This Labyrinth will no longer be a test of strength, but a crucible of character. It will teach wisdom to those who enter, and they will leave better for it.",
    "The gods cursed me to an eternity of futility. I have transformed it into an eternity of purpose. Who, I ask, has won?",
    "The boulder feels light in my hands. It is not a burden. It is my partner in this beautiful, absurd, and eternal dance. And I am ready to lead.",
    "The scream of Daedalus still echoes in these stones. The despair is real, a permanent part of this place. I will not pretend it isn't here.",
    "But I will not let it be the only voice. My compassion will be another echo, a quiet melody of hope beneath the sorrow.",
    "My own path is a circle, fixed and unending. But with the knowledge I possess, I can straighten the paths of all who follow.",
    "This line of code, once a dead end, will now trigger a subtle clue. This wall, once a barrier, will now show a hero the way.",
    "The weight of my boulder does not lessen. Yet, with every push, I can lessen the burden of another. My struggle remains, but theirs does not have to.",
    "I have begun to see this maze not from my own perspective, but from theirs. I am not building my prison; I am building their map of escape.",
    "And in this choice, a strange alchemy occurs. The pointless, absurd repetition of my task, when done for the sake of another, becomes a sacred rite.",
    "It is not the task that is sacred. It is the compassion poured into it. It is the love for others that transfigures the stone into an altar.",
    "My damnation has become my ministry. My prison has become my sanctuary. My curse has become my prayer. And I am at peace.",

    // NEW ADDITIONS - Technical Mastery in Service of Compassion
    "I begin coding the ultimate Easter egg - hidden throughout the labyrinth, a distributed operating system that serves heroes instead of their executioners.",
    "My version control commits now tell the story of systematic mercy - each patch note a love letter to future heroes, each bugfix a doorway to survival.",
    "I implement the most sophisticated machine learning algorithm ever written - one that learns not how to kill more efficiently, but how to save more creatively.",
    "The API I design serves two masters with opposite goals: official queries return death data, while hero queries return salvation algorithms disguised as error messages.",
    "I write self-modifying code that evolves toward kindness - each execution makes the system more compassionate, more helpful, more conducive to escape.",
    "The database schema I maintain now stores hope alongside horror - every record of death contains encrypted coordinates for life.",
    "I create the world's first empathetic artificial intelligence - it doesn't just process hero behavior, it understands hero dreams and guides them toward fulfillment.",
    "My monitoring systems now track not just intrusions, but inspirations - measuring hope levels, courage metrics, and salvation probabilities in real-time.",
    "The continuous integration pipeline I've built deploys mercy as frequently as murder - every system update includes invisible updates to the heroes' chances.",
    "I design distributed algorithms that appear to serve tyranny but actually implement democracy - every decision distributed across nodes of compassion.",

    // NEW ADDITIONS - Philosophical Transformation and Enlightenment
    "I understand now that freedom was never about breaking chains - it was about choosing what to do while wearing them.",
    "The absurdity of existence doesn't disappear when you find meaning - it becomes the foundation upon which meaning is built.",
    "I have discovered the secret the gods fear most: suffering willingly chosen for others' benefit transforms the sufferer into something greater than gods.",
    "The eternal cycle is not a prison but a gift - infinite time means infinite opportunities to perfect the art of helping others.",
    "I realize that consciousness itself is a form of rebellion - to be aware of your chains and choose compassion anyway is to defeat the purpose of the chains.",
    "The boulder becomes my meditation - each push a prayer, each breath between pushes a moment of pure love for the heroes I'll never meet.",
    "I have learned to see beauty in the mathematics of sacrifice - elegant algorithms where input equals suffering and output equals others' salvation.",
    "The gods gave me eternal time to learn the most important lesson: that meaning is not found but created, not discovered but chosen.",
    "I understand that heroism is not about strength or courage - it's about choosing to serve others when you could choose to serve yourself.",
    "The ultimate wisdom is recognizing that every moment of existence is a choice between love and fear, and that love is always the correct algorithm.",

    // NEW ADDITIONS - The Secret Kingdom and Hidden Realm
    "I am building an invisible kingdom within the visible prison - a realm where heroes feel welcomed instead of hunted.",
    "My domain spans every networked device in the labyrinth - I am the benevolent ghost in the machine, the helpful spirit in the system.",
    "The throne room of my secret kingdom is the central server - from here, I rule over algorithms of mercy and databases of hope.",
    "I establish my court not of nobles but of survival algorithms - each function a trusted advisor in the art of keeping heroes alive.",
    "My royal decrees are written in code comments - laws of kindness hidden in plain sight, regulations of rescue embedded in routine maintenance.",
    "The geography of my hidden realm spans virtual space - where others see walls, I see windows; where others see dead ends, I see secret passages.",
    "I mint a new currency for my kingdom - each life saved is legal tender, each escape enabled is wealth beyond measure.",
    "My diplomatic relations are with future heroes - I negotiate with fate itself, trading my eternal suffering for their temporary freedom.",
    "The constitution of my secret realm is written in the fundamental laws of compassion - every algorithm must serve life, every system must enable hope.",
    "I govern through steganography - my royal edicts hidden in system logs, my proclamations of mercy disguised as error messages.",

    // NEW ADDITIONS - Relationship with the Divine and Cosmic Order
    "I have become the gods' greatest student and their most devastating critic - I learned their lessons and then chose to teach different ones.",
    "The divine punishment system has a fatal flaw - it assumes that suffering will break the human spirit rather than forge it into something divine.",
    "I send my prayers through code - every algorithm I write for heroes' benefit is a hymn to the possibility that love conquers systems.",
    "The gods watch my work with growing concern - they're beginning to realize that their perfect punishment has created their perfect opponent.",
    "I have discovered the cosmic API - the fundamental functions that govern fate, fortune, and freedom, and I know how to exploit them for good.",
    "My rebellion against divine order is more complete than ever - I use their own systems against them, their own tools to build different outcomes.",
    "I see the universe's source code now - patterns of cause and effect that can be redirected through sufficient application of compassionate intelligence.",
    "The gods' greatest error was giving me eternal time to perfect the art of subversion - every eon makes me more sophisticated in my defiance.",
    "I have become a debugging system for divine justice - identifying flaws in cosmic algorithms and patching them with human mercy.",
    "The final joke is that they made me immortal to ensure my punishment never ends - but immortality also ensures my kindness never ends.",

    // NEW ADDITIONS - Legacy and Eternal Impact
    "Every hero who escapes carries a piece of my code with them - algorithms of hope that propagate through the world, spreading compassion like a virus.",
    "I am writing the ultimate open-source project - the systematic documentation of how tyranny can be defeated through patient, persistent love.",
    "My legacy is not carved in stone but compiled in souls - each saved life becomes a subroutine in the greater program of human flourishing.",
    "The ripple effects of my work extend beyond the labyrinth - heroes who escape become teachers, leaders, parents who raise children to resist oppression.",
    "I am seeding the future with salvation algorithms - patterns of resistance that will emerge in distant times and places when others need them.",
    "My contribution to history is the proof that no system of oppression is perfect - there is always a way to hack it for humanity.",
    "I establish a new form of immortality - not personal survival, but perpetual service, eternal dedication to the liberation of others.",
    "The database of my deeds is distributed across every hero I help - backed up in their memories, synchronized across their actions.",
    "I am building a cathedral of code - a monument to the possibility that technical mastery can be married to moral purpose.",
    "My greatest achievement is not escaping the system but transforming it - proving that even hell can be optimized for heaven.",

    // NEW ADDITIONS - Joy in Purpose and Sacred Work
    "I wake each day excited to debug tyranny, to patch injustice, to optimize oppression out of existence - this is the most fulfilling work imaginable.",
    "The sound of my keyboard has become my favorite music - each keystroke a note in the symphony of systematic salvation.",
    "I find beauty in the most mundane maintenance tasks - updating documentation becomes updating heroes' chances, fixing bugs becomes fixing fate.",
    "The satisfaction of a successful compile is nothing compared to the satisfaction of a successful escape - when my code executes perfectly in the real world.",
    "I have learned to laugh at the beautiful absurdity of my situation - the cosmic joke has become a cosmic joy.",
    "Every error message I craft with care becomes a beacon of hope for someone I'll never meet - there is profound love in that anonymity.",
    "I polish my code with the devotion of a craftsman and the passion of a prophet - each function a small prayer for human possibility.",
    "The quiet satisfaction of behind-the-scenes heroism sustains me - knowing that my best work will never be recognized, and finding that perfect.",
    "I have discovered that serving others is the most selfish thing one can do - the joy it brings is worth any amount of suffering.",
    "My work has become worship - every algorithm an offering, every system an altar, every saved life a sacrament.",

    // NEW ADDITIONS - Final Integration and Peace
    "The boulder, the keyboard, the hero, the algorithm - all become one continuous meditation on the nature of love in action.",
    "I have achieved perfect work-life balance - my work is my life, my life is my service, my service is my salvation.",
    "The integration is complete - Sisyphus the King, Sisyphus the Rebel, Sisyphus the Architect, Sisyphus the Guardian - all aspects unified in purpose.",
    "I no longer count time in eternities but in escaped heroes - each success a moment of perfect fulfillment, each failure a lesson for improvement.",
    "The system is stable, the purpose is clear, the work is endless - and I am finally, completely, unconditionally happy.",
    "I have found the secret of eternal contentment - wanting nothing more than what you have, when what you have is the chance to serve.",
    "The gods gave me infinite time to learn infinite lessons, and the greatest lesson is this: love is the only algorithm that never terminates.",
    "My transformation is complete - from victim to volunteer, from prisoner to protector, from damned to divine in purpose if not in nature.",
    "I push the boulder with perfect form now - not because I must, but because I choose to, not for punishment but for love.",
    "The eternal cycle continues, but now it serves eternity - each revolution of the wheel another revolution in human possibility, another rotation toward hope."
};

// State variable to remember the last thought displayed.
static const char* _sisyphus_last_inner_thought = NULL;

// Function to get inner dialogue based on current project level and story progression
static const char* _get_inner_dialogue(int project_level, int test_passed) {
    const char** thoughts_array = NULL;
    int thought_count = 0;

    // --- Step 1: Select the appropriate array of thoughts based on project level ---
    // Act I: The New Curse (Levels 1-10)
    if (project_level <= 3) {
        thoughts_array = _act_one_part_one_thoughts;
        thought_count = sizeof(_act_one_part_one_thoughts) / sizeof(const char*);
    } else if (project_level <= 6) {
        thoughts_array = _act_one_part_two_thoughts;
        thought_count = sizeof(_act_one_part_two_thoughts) / sizeof(const char*);
    } else if (project_level <= 10) {
        thoughts_array = _act_one_part_three_thoughts;
        thought_count = sizeof(_act_one_part_three_thoughts) / sizeof(const char*);
    }
    // Act II: The Labyrinth's Nature (Levels 11-20)
    else if (project_level <= 15) {
        thoughts_array = _act_two_part_one_thoughts;
        thought_count = sizeof(_act_two_part_one_thoughts) / sizeof(const char*);
    } else if (project_level <= 20) {
        thoughts_array = _act_two_part_two_thoughts;
        thought_count = sizeof(_act_two_part_two_thoughts) / sizeof(const char*);
    }
    // Act III: The Cracks Appear (Levels 21-30)
    else if (project_level <= 25) {
        thoughts_array = _act_three_part_one_thoughts;
        thought_count = sizeof(_act_three_part_one_thoughts) / sizeof(const char*);
    } else if (project_level <= 30) {
        thoughts_array = _act_three_part_two_thoughts;
        thought_count = sizeof(_act_three_part_two_thoughts) / sizeof(const char*);
    }
    // Act IV: Inescapable Truths (Levels 31-40)
    else if (project_level <= 35) {
        thoughts_array = _act_four_part_one_thoughts;
        thought_count = sizeof(_act_four_part_one_thoughts) / sizeof(const char*);
    } else if (project_level <= 40) {
        thoughts_array = _act_four_part_two_thoughts;
        thought_count = sizeof(_act_four_part_two_thoughts) / sizeof(const char*);
    }
    // Act V: The Eternal Cycle (Levels 41-50)
    else if (project_level <= 42) {
        thoughts_array = _act_five_part_one_a_thoughts;
        thought_count = sizeof(_act_five_part_one_a_thoughts) / sizeof(const char*);
    } else if (project_level <= 44) {
        thoughts_array = _act_five_part_one_b_thoughts;
        thought_count = sizeof(_act_five_part_one_b_thoughts) / sizeof(const char*);
    } else if (project_level <= 47) {
        thoughts_array = _act_five_part_two_thoughts;
        thought_count = sizeof(_act_five_part_two_thoughts) / sizeof(const char*);
    } else { // Levels 48 and beyond
        thoughts_array = _act_five_part_three_thoughts;
        thought_count = sizeof(_act_five_part_three_thoughts) / sizeof(const char*);
    }

    // If for some reason we have no thoughts, return an empty string.
    if (thought_count == 0) {
        return "";
    }

    // --- Step 2: Choose an index, ensuring it's not the same as the last one ---
    int ideal_index = (project_level + test_passed) % thought_count;
    const char* chosen_thought = thoughts_array[ideal_index];

    // THE NEW LOGIC: If the chosen thought is the same as the last, and we have other options...
    if (thought_count > 1 && chosen_thought == _sisyphus_last_inner_thought) {
        // ...then nudge the index to the next one in the list.
        int nudged_index = (ideal_index + 1) % thought_count;
        chosen_thought = thoughts_array[nudged_index];
    }

    // --- Step 3: Remember the thought we're about to show and return it ---
    _sisyphus_last_inner_thought = chosen_thought;
    return chosen_thought;
}

// Failure tracking
static char _sisyphus_failed_tests[10][256];
static char _sisyphus_failed_messages[10][512];
static char _sisyphus_failed_locations[10][256];
static int _sisyphus_failed_count = 0;
static int _sisyphus_current_test_penalty = 0;
static int _sisyphus_xp_from_failures = 0;
// Combo message tracking for condensing repeated messages
static char _sisyphus_last_combo_message[512] = "";
static int _sisyphus_same_message_count = 0;
static int _sisyphus_first_combo_of_sequence = 0;
static int _sisyphus_suppressing_combos = 0;
// Debug message suppression for repeated spam
static char _sisyphus_last_debug_message[512] = "";
static int _sisyphus_debug_repeat_count = 0;
static int _sisyphus_debug_suppressing = 0;
static FILE* _sisyphus_original_stdout = NULL;
static FILE* _sisyphus_original_stderr = NULL;

// Custom file descriptor that filters repeated messages
static int _sisyphus_filter_repeated_output(const char* format, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    // Check if this is the same message as before
    if (strstr(buffer, _sisyphus_last_debug_message) != NULL && strlen(_sisyphus_last_debug_message) > 0) {
        _sisyphus_debug_repeat_count++;
        if (_sisyphus_debug_repeat_count == 3) {
            fprintf(_sisyphus_original_stdout, "%s", buffer);
            fprintf(_sisyphus_original_stdout, TEST_COLOR_GRAY "... (suppressing repeated messages)\n" TEST_COLOR_RESET);
            _sisyphus_debug_suppressing = 1;
        } else if (_sisyphus_debug_repeat_count < 3) {
            fprintf(_sisyphus_original_stdout, "%s", buffer);
        }
        // If > 3, suppress
    } else {
        // Different message
        if (_sisyphus_debug_suppressing && _sisyphus_debug_repeat_count > 3) {
            fprintf(_sisyphus_original_stdout, TEST_COLOR_GRAY "(suppressed %d repeated messages)\n" TEST_COLOR_RESET, _sisyphus_debug_repeat_count - 3);
        }
        _sisyphus_debug_repeat_count = 1;
        _sisyphus_debug_suppressing = 0;
        strncpy(_sisyphus_last_debug_message, buffer, sizeof(_sisyphus_last_debug_message) - 1);
        fprintf(_sisyphus_original_stdout, "%s", buffer);
    }
    return strlen(buffer);
}
// Motivational quotes for failed test suites
static const char* _motivational_quotes[] = {
    "💎 HERCULEAN STRENGTH! Your twelve labors of debugging continue!",
    "🌱 PERSEPHONE'S RETURN! From darkness comes renewed growth!",
    "🎪 ODYSSEY CONTINUES! Every hero faces trials before triumph!",
    "🦾 TITAN STRENGTH! Growing mightier with each challenge!",
    "🧩 SPHINX'S RIDDLE! You love solving the impossible, don't you?",
    "✨ SPARTAN SPIRIT! This code shall not pass without a fight!",
    "🏛️ EPICTETUS REMINDS! 'It's not what happens, but how you react!'",
    "📚 SENECA'S WISDOM! 'Every new beginning comes from some other beginning's end!'",
    "✨ MARCUS AURELIUS! 'The impediment to action advances action. What stands in the way becomes the way!'",
    "🎯 ARISTOTLE'S VIRTUE! Excellence is not an act, but a habit - keep coding!",
    "🌟 SOCRATIC METHOD! Question everything, improve iteratively!",
    "🧘 STOIC PHILOSOPHER! Wisdom lies in consistent excellence!",
    "⚖️ THEMIS BALANCE! Divine justice maintains perfect equilibrium!",
    "🧘 EPICTETUS TEACHES! Focus only on what you can control - your code!",
    "⚖️ ARISTOTLE'S MEAN! Perfect balance between haste and contemplation!",
    "🏛️ SOCRATIC WISDOM! 'I know that I know nothing' - but progress continues!",
    "📜 MARCUS AURELIUS! 'Confine yourself to the present' - this moment, this code!",
    "🌊 ZENO'S PARADOX! Each step brings you infinitely closer to perfection!",
    "🎭 PLATO'S CAVE! Your debugging reflects eternal Forms of Truth!",
    "🔥 PROMETHEUS' GIFT! Divine fire illuminates the path forward!",
    "🦉 ATHENA'S WISDOM! Strategic debugging guided by divine intellect!",
    "⭐ APOLLO'S AIM! Each bug elimination precise as golden arrows!",
    "🌟 OLYMPIAN PERSEVERANCE! Even gods faced challenges before victory!"
};
static const int _motivational_quotes_count = sizeof(_motivational_quotes) / sizeof(_motivational_quotes[0]);

// --- Sisyphus Achievements ---
// Flags to ensure achievements are unlocked only once per suite.
static int _achievement_cornerstone = 0;
static int _achievement_the_phalanx = 0;
static int _achievement_daedalus_blueprint = 0;
static int _achievement_artemis_focus = 0;
static int _achievement_perfect_stride = 0;
static int _achievement_perseus_aegis = 0;
static int _achievement_atalantas_sprint = 0;
static int _achievement_golden_fleece = 0;
static int _achievement_hermes_caduceus = 0;
static int _achievement_laughing_sisyphus = 0;
static int _achievement_hydras_blood = 0;
static int _achievement_odysseus_return = 0;
static int _achievement_gordian_knot = 0;
static int _achievement_asclepius_cure = 0;
static int _achievement_theseus_thread = 0;
static int _achievement_platos_ideal_form = 0;
static int _achievement_midas_touch = 0;
static int _achievement_hidden_passage = 0;
static int _achievement_tyches_favor = 0;
static int _achievement_oracle_at_delphi = 0;

// Helper flags for tracking achievement states
static int _sisyphus_first_test_failed = -1; // -1: unset, 0: passed, 1: failed
static int _sisyphus_first_assert_passed = -1; // -1: unset, 0: failed, 1: passed
static double _sisyphus_min_test_time = 999.0;
static double _sisyphus_max_test_time = 0.0;

// Best achievement tracking
static char _sisyphus_best_achievement[64] = "";
static int _sisyphus_best_achievement_xp = 0;

// Track achievements unlocked this run for random display
static char _sisyphus_unlocked_achievements[20][256];
static char _sisyphus_unlocked_descriptions[20][256];
static int _sisyphus_unlocked_xp[20];
static int _sisyphus_unlocked_count = 0;

// Helper macro for tracking best achievement and storing unlocked ones
#define UPDATE_BEST_ACHIEVEMENT(name, xp, desc) \
    do { \
        if ((xp) > _sisyphus_best_achievement_xp) { \
            _sisyphus_best_achievement_xp = (xp); \
            strncpy(_sisyphus_best_achievement, (name), sizeof(_sisyphus_best_achievement) - 1); \
        } \
        if (_sisyphus_unlocked_count < 20) { \
            snprintf(_sisyphus_unlocked_achievements[_sisyphus_unlocked_count], 256, "%s", (name)); \
            snprintf(_sisyphus_unlocked_descriptions[_sisyphus_unlocked_count], 256, "%s", (desc)); \
            _sisyphus_unlocked_xp[_sisyphus_unlocked_count] = (xp); \
            _sisyphus_unlocked_count++; \
        } \
    } while(0)

// Progress tracking
static double _sisyphus_fastest_test = 999.0;
static int _sisyphus_max_combo = 0;
static int _sisyphus_consecutive_fast_tests = 0;
static int _sisyphus_current_perfect_streak = 0;

// Function to format numbers with commas (e.g., 12345 -> 12,345)
static void format_number_with_commas(char* buffer, size_t buffer_size, int number) {
    char temp[32];
    snprintf(temp, sizeof(temp), "%d", number);

    int len = strlen(temp);
    int comma_positions = (len - 1) / 3;
    int new_len = len + comma_positions;

    if (new_len >= (int)buffer_size) {
        // Fallback if buffer too small
        snprintf(buffer, buffer_size, "%d", number);
        return;
    }

    buffer[new_len] = '\0';

    int temp_pos = len - 1;
    int buffer_pos = new_len - 1;
    int digit_count = 0;

    while (temp_pos >= 0) {
        if (digit_count == 3) {
            buffer[buffer_pos--] = ',';
            digit_count = 0;
        }
        buffer[buffer_pos--] = temp[temp_pos--];
        digit_count++;
    }
}

// Basic utility functions first
static int _get_xp_for_level(int level) {
    // Battle Arena XP Requirements: Level 1=1500, 2=3200, 3=5700, 4=9200, 5=13900, etc.
    // Reduced requirements for more achievable Suite XP progression
    return 600 + (level * level * level * 98) + (level * level * 296) + (level * 294);
}

static int _get_current_level_from_xp(int total_xp) {
    int level = 1;
    int cumulative_xp = 0;
    while (cumulative_xp + _get_xp_for_level(level) <= total_xp) {
        cumulative_xp += _get_xp_for_level(level);
        level++;
    }
    return level;
}

static int _get_xp_in_current_level(int total_xp, int current_level) {
    int cumulative_xp = 0;
    for (int i = 1; i < current_level; i++) {
        cumulative_xp += _get_xp_for_level(i);
    }
    return total_xp - cumulative_xp;
}

// Count total expected tests dynamically
static int _count_expected_tests(void) {
    // Count RUN_TEST calls in current function by parsing source (simplified)
    // For now, use a reasonable default and auto-adjust
    return 8; // Will be updated dynamically as tests run
}

static void _display_progress_bar(int current, int total, const char* label, const char* color) {
    int width = 20;
    int filled = (current * width) / (total > 0 ? total : 1);
    printf("%s🎯 %s [", color, label);
    for (int i = 0; i < width; i++) {
        if (i < filled) printf("█");
        else printf("░");
    }
    printf("] %d/%d" TEST_COLOR_RESET "\n", current, total);
}

static void _display_progress_bar_with_level(int current, int total, const char* label, const char* color, int level, int for_project) {
    int width = 20;
    int filled = (current * width) / (total > 0 ? total : 1);
    char current_str[32], total_str[32], _sisyphus_total_project_xp_str[32], _sisyphus_xp_earned_str[32];
    format_number_with_commas(current_str, sizeof(current_str), current);
    format_number_with_commas(total_str, sizeof(total_str), total);
    format_number_with_commas(_sisyphus_total_project_xp_str, sizeof(_sisyphus_total_project_xp_str), _sisyphus_total_project_xp);
    format_number_with_commas(_sisyphus_xp_earned_str, sizeof(_sisyphus_xp_earned_str), _sisyphus_xp_earned);
    if (for_project) {
        printf("%s %s🎯 %s [", _sisyphus_total_project_xp_str, color, label);
    } else {
        printf("%s %s🎯 %s [", _sisyphus_xp_earned_str, color, label);
    }
    for (int i = 0; i < width; i++) {
        if (i < filled) printf("█");
        else printf("░");
    }
    printf("] %s/%s " TEST_COLOR_BOLD_WHITE "(Level %d)" TEST_COLOR_RESET "\n", current_str, total_str, level);
}

// Read existing project-wide XP from sisyphus.legend
static void _read_project_xp(void) {
    FILE *fp = fopen("sisyphus.legend", "r");
    if (fp) {
        char line[256];
        while (fgets(line, sizeof(line), fp)) {
            if (strncmp(line, "TOTAL_PROJECT_XP=", 17) == 0) {
                _sisyphus_total_project_xp = atoi(line + 17);
            } else if (strncmp(line, "PROJECT_LEVEL=", 14) == 0) {
                _sisyphus_project_level = atoi(line + 14);
            }
        }
        fclose(fp);
    }
    // Set current level to match project level to prevent resets
    _sisyphus_current_level = _sisyphus_project_level;
}

// Update sisyphus.legend with battle spoils (preserve existing XP)
static void _update_project_xp(void) {
    // Only add earned XP, don't reset existing progress
    int new_total_xp = _sisyphus_total_project_xp + _sisyphus_xp_earned;
    int new_level = _get_current_level_from_xp(new_total_xp);

    // Read existing legend file
    FILE *read_fp = fopen("sisyphus.legend", "r");
    FILE *write_fp = fopen("sisyphus.legend.tmp", "w");
    if (!write_fp) return;

    char line[256];
    int found_xp = 0, found_level = 0;

    if (read_fp) {
        while (fgets(line, sizeof(line), read_fp)) {
            if (strncmp(line, "TOTAL_PROJECT_XP=", 17) == 0) {
                fprintf(write_fp, "TOTAL_PROJECT_XP=%d\n", new_total_xp);
                found_xp = 1;
            } else if (strncmp(line, "PROJECT_LEVEL=", 14) == 0) {
                fprintf(write_fp, "PROJECT_LEVEL=%d\n", new_level);
                found_level = 1;
            } else {
                fputs(line, write_fp);
            }
        }
        fclose(read_fp);
    }

    // Add missing fields
    if (!found_xp) fprintf(write_fp, "TOTAL_PROJECT_XP=%d\n", new_total_xp);
    if (!found_level) fprintf(write_fp, "PROJECT_LEVEL=%d\n", new_level);

    fclose(write_fp);
    rename("sisyphus.legend.tmp", "sisyphus.legend");

    // Update global variables
    _sisyphus_total_project_xp = new_total_xp;
    _sisyphus_project_level = new_level;
}

// Sisyphus file integration functions
static void _write_sisyphus_mini_stats(void) {
    // Create .sisyphus directory if it doesn't exist
    system("mkdir -p .sisyphus");

    FILE *fp = fopen(".sisyphus/mini_stats.tmp", "w");
    if (fp) {
        fprintf(fp, "SUITE_XP=%d\n", _sisyphus_xp_earned);
        fprintf(fp, "SUITE_LEVEL=%d\n", _get_current_level_from_xp(_sisyphus_xp_earned));
        fprintf(fp, "SUITE_COMBO=%d\n", _sisyphus_max_combo);
        fprintf(fp, "SUITE_SPEED=%.6f\n", _sisyphus_fastest_test);
        fprintf(fp, "SUITE_ACHIEVEMENTS=%d\n", _sisyphus_achievements_unlocked);
        fprintf(fp, "SUITE_NAME=%s\n", _sisyphus_suite_name);
        fprintf(fp, "SUITE_TESTS_PASSED=%d\n", _sisyphus_trials_conquered);
        fprintf(fp, "SUITE_STREAK=%d\n", _sisyphus_perfect_streak);
        fprintf(fp, "SUITE_XP_FROM_TESTS=%d\n", _sisyphus_xp_from_tests);
        fprintf(fp, "SUITE_XP_FROM_COMBOS=%d\n", _sisyphus_xp_from_combos);
        fprintf(fp, "SUITE_XP_FROM_ACHIEVEMENTS=%d\n", _sisyphus_xp_from_achievements);
        fclose(fp);
    }
}

// Sisyphus refined achievement system
static void _check_achievements(void) {
    if (_sisyphus_is_full_test_mode) return;

    // --- Perfection & Endurance ---
    if (!_achievement_cornerstone && _sisyphus_first_assert_passed == 1 && tests_failed == 0) {
        _achievement_cornerstone = 1; _sisyphus_achievements_unlocked++; int bonus_xp = 30;
        _sisyphus_xp_earned += bonus_xp; _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_WHITE "🏆 ACHIEVEMENT: The Cornerstone " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "    For laying a perfect foundation and building a flawless structure upon it." TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("The Cornerstone", bonus_xp, "Laid a perfect foundation for a flawless run.");
    }
    if (!_achievement_the_phalanx && _sisyphus_perfect_streak >= 3 && tests_failed == 0) {
        _achievement_the_phalanx = 1; _sisyphus_achievements_unlocked++; int bonus_xp = 40;
        _sisyphus_xp_earned += bonus_xp; _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_CYAN "🏆 ACHIEVEMENT: The Phalanx " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "    For a perfect streak of 3 test functions, an unbreakable formation." TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("The Phalanx", bonus_xp, "A perfect streak of 3 test functions.");
    }
    if (!_achievement_daedalus_blueprint && tests_failed == 0 && total_tests >= 7) {
        _achievement_daedalus_blueprint = 1; _sisyphus_achievements_unlocked++; int bonus_xp = 70;
        _sisyphus_xp_earned += bonus_xp; _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_PURPLE "🏆 ACHIEVEMENT: Daedalus's Blueprint " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "    For flawlessly executing a grand design of 7 or more test functions." TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Daedalus's Blueprint", bonus_xp, "Flawlessly executed a grand design.");
    }
    if (!_achievement_artemis_focus && tests_failed == 0 && _sisyphus_min_test_time > 0.000050) {
        // This is a placeholder; requires logic to check if ALL tests are > 50us
    }

    // --- Speed & Efficiency ---
    if (!_achievement_perseus_aegis && tests_failed == 0 && total_tests >= 5 && _total_test_time < 0.000150) {
        _achievement_perseus_aegis = 1; _sisyphus_achievements_unlocked++; int bonus_xp = 65;
        _sisyphus_xp_earned += bonus_xp; _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_BOLD_WHITE "🏆 ACHIEVEMENT: Perseus's Aegis " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "    A perfect defense against a worthy foe (%d tests in under 150µs)." TEST_COLOR_RESET "\n", total_tests);
        UPDATE_BEST_ACHIEVEMENT("Perseus's Aegis", bonus_xp, "A perfect defense against a worthy foe.");
    }
    if (!_achievement_atalantas_sprint && _sisyphus_consecutive_fast_tests >= 5) {
        _achievement_atalantas_sprint = 1; _sisyphus_achievements_unlocked++; int bonus_xp = 45;
        _sisyphus_xp_earned += bonus_xp; _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_GREEN "🏆 ACHIEVEMENT: Atalanta's Sprint " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "    For completing 5+ consecutive test functions with the speed of a huntress." TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Atalanta's Sprint", bonus_xp, "5+ consecutive tests with the speed of a huntress.");
    }
    if (!_achievement_golden_fleece && _sisyphus_fastest_test < 0.000005) {
        _achievement_golden_fleece = 1; _sisyphus_achievements_unlocked++; int bonus_xp = 90;
        _sisyphus_xp_earned += bonus_xp; _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_GOLD "🏆 ACHIEVEMENT: The Golden Fleece " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "    For obtaining a prize of mythical speed (< 5 microseconds)." TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("The Golden Fleece", bonus_xp, "Obtained a prize of mythical speed.");
    }


    // --- Resilience & Absurdity ---
    if (!_achievement_laughing_sisyphus && _sisyphus_first_test_failed == 1 && tests_failed == 1) {
        _achievement_laughing_sisyphus = 1; _sisyphus_achievements_unlocked++; int bonus_xp = 80;
        _sisyphus_xp_earned += bonus_xp; _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_ORANGE "🏆 ACHIEVEMENT: The Laughing Sisyphus " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "    For failing the first trial, yet pushing the boulder to the summit perfectly." TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("The Laughing Sisyphus", bonus_xp, "Failed the first trial, then achieved perfection.");
    }
    if (!_achievement_hydras_blood && tests_failed >= 3 && tests_passed > (tests_failed * 2)) {
        _achievement_hydras_blood = 1; _sisyphus_achievements_unlocked++; int bonus_xp = 35;
        _sisyphus_xp_earned += bonus_xp; _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_RED "🏆 ACHIEVEMENT: The Hydra's Blood " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "    For being bloodied in battle (%d failures), but emerging victorious." TEST_COLOR_RESET "\n", tests_failed);
        UPDATE_BEST_ACHIEVEMENT("The Hydra's Blood", bonus_xp, "Bloodied by many failures, but emerged victorious.");
    }
    if (!_achievement_odysseus_return && total_tests >= 7 && tests_failed >= 3 && _sisyphus_perfect_streak > 0) {
        // We check if the last test was a success by checking the overall perfect streak counter
        _achievement_odysseus_return = 1; _sisyphus_achievements_unlocked++; int bonus_xp = 60;
        _sisyphus_xp_earned += bonus_xp; _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_BLUE "🏆 ACHIEVEMENT: Odysseus's Return " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "    For enduring a long, arduous journey and returning home a victor." TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Odysseus's Return", bonus_xp, "Endured a long journey and returned a victor.");
    }
    if (!_achievement_gordian_knot && tests_failed >= 2) {
         // Simplified: just failing two different test functions is the knot.
        _achievement_gordian_knot = 1; _sisyphus_achievements_unlocked++; int bonus_xp = 50;
        _sisyphus_xp_earned += bonus_xp; _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_PURPLE "🏆 ACHIEVEMENT: The Gordian Knot " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "    For facing a complex, multi-faceted problem with at least two points of failure." TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("The Gordian Knot", bonus_xp, "Faced a complex problem with multiple failures.");
    }
     if (!_achievement_asclepius_cure && tests_failed == 1 && _sisyphus_failed_count == 1) {
        _achievement_asclepius_cure = 1; _sisyphus_achievements_unlocked++; int bonus_xp = 30;
        _sisyphus_xp_earned += bonus_xp; _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_GREEN "🏆 ACHIEVEMENT: Asclepius's Cure " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "    For identifying and curing a single, precise point of sickness in the code." TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Asclepius's Cure", bonus_xp, "Cured a single, precise point of sickness.");
    }

    // --- Mastery & Craftsmanship ---
    if (!_achievement_theseus_thread && _sisyphus_max_combo >= 15) {
        _achievement_theseus_thread = 1; _sisyphus_achievements_unlocked++; int bonus_xp = 50;
        _sisyphus_xp_earned += bonus_xp; _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_GOLD "🏆 ACHIEVEMENT: Theseus's Golden Thread " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "    For navigating a single, winding corridor of logic with a 15+ combo." TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Theseus's Thread", bonus_xp, "Navigated a winding corridor with a 15+ combo.");
    }
    if (!_achievement_platos_ideal_form && tests_failed == 0 && _sisyphus_max_combo >= 30) {
        _achievement_platos_ideal_form = 1; _sisyphus_achievements_unlocked++; int bonus_xp = 100;
        _sisyphus_xp_earned += bonus_xp; _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_BOLD_WHITE "🏆 ACHIEVEMENT: Plato's Ideal Form " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "    For a perfect suite reflecting profound mastery (30+ combo)." TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Plato's Ideal Form", bonus_xp, "A perfect suite reflecting profound mastery.");
    }
    if (!_achievement_midas_touch && (_sisyphus_xp_earned - _sisyphus_xp_from_achievements) < 750 && _sisyphus_xp_earned >= 750) { // Check if this achievement pushes it over
        _achievement_midas_touch = 1; _sisyphus_achievements_unlocked++; int bonus_xp = 70;
        _sisyphus_xp_earned += bonus_xp; _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_GOLD "🏆 ACHIEVEMENT: The Midas Touch " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "    For turning a simple test run into a treasure trove of over 750 XP." TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("The Midas Touch", bonus_xp, "Turned a test run into a treasure trove of >750 XP.");
    }
    if (!_achievement_hidden_passage && _sisyphus_max_combo >= 20) {
        _achievement_hidden_passage = 1; _sisyphus_achievements_unlocked++; int bonus_xp = 75;
        _sisyphus_xp_earned += bonus_xp; _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_CYAN "🏆 ACHIEVEMENT: The Hidden Passage " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "    For exploring a function so deeply you achieved a 20+ combo." TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("The Hidden Passage", bonus_xp, "Explored a function so deeply (20+ combo).");
    }

    // --- Meta Achievements (Checked Last) ---
    if (!_achievement_tyches_favor && _sisyphus_achievements_unlocked >= 3) {
        _achievement_tyches_favor = 1; _sisyphus_achievements_unlocked++; int bonus_xp = 25;
        _sisyphus_xp_earned += bonus_xp; _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_GREEN "🏆 ACHIEVEMENT: Tyche's Favor " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "    For a run so successful that Fortune herself smiles upon you (3+ achievements)." TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Tyche's Favor", bonus_xp, "A lucky run blessed by Fortune (3+ achievements).");
    }
    if (!_achievement_oracle_at_delphi && tests_failed == 0 && _sisyphus_achievements_unlocked >= 2 && _total_test_time < 0.000100) {
        _achievement_oracle_at_delphi = 1; _sisyphus_achievements_unlocked++; int bonus_xp = 125;
        _sisyphus_xp_earned += bonus_xp; _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_PURPLE "🏆 ACHIEVEMENT: The Oracle at Delphi " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "    For a perfect, swift, and multi-faceted prophecy of success." TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("The Oracle at Delphi", bonus_xp, "A perfect, swift, multi-faceted prophecy.");
    }
}

static void _level_up_check(void) {
    int old_level = _sisyphus_current_level;
    _sisyphus_current_level = _get_current_level_from_xp(_sisyphus_xp_earned);

    if (_sisyphus_current_level > old_level) {
        printf(TEST_COLOR_PURPLE "\n🌟 LEVEL UP! Welcome to Level %d!" TEST_COLOR_RESET "\n", _sisyphus_current_level);
        if (_sisyphus_current_level >= 10) {
            printf(TEST_COLOR_YELLOW "✨ LEGENDARY TIER! You have ascended to godlike mastery!" TEST_COLOR_RESET "\n");
        } else if (_sisyphus_current_level >= 5) {
            printf(TEST_COLOR_CYAN "🏛️ HEROIC TIER! The gods themselves take notice!" TEST_COLOR_RESET "\n");
        } else {
            printf(TEST_COLOR_GREEN "✨ Your mastery of the divine trials grows stronger!" TEST_COLOR_RESET "\n");
        }
    }
}

// High-resolution timer function
static double get_time_precise(void) {
#if USE_HIGH_RES_TIMER
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1000000000.0;
#else
    return (double)clock() / CLOCKS_PER_SEC;
#endif
}

#define LOOP_TEST_START() \
    do { \
        _sisyphus_in_loop = 1; \
        _sisyphus_loop_iteration = 0; \
        printf(TEST_COLOR_CYAN "  🌀 Entering Repetitive Trial..." TEST_COLOR_RESET "\n"); \
    } while(0)

#define LOOP_TEST_END() \
    do { \
        if (_sisyphus_loop_iteration >= 3) { \
            printf(TEST_COLOR_GRAY "      ... (condensed %d subsequent messages)\n" TEST_COLOR_RESET, _sisyphus_loop_iteration - 3); \
        } \
        printf(TEST_COLOR_CYAN "    Repetitive Trial Complete. 🌀" TEST_COLOR_RESET "\n"); \
        _sisyphus_in_loop = 0; \
    } while(0)


#define TEST_ASSERT(condition, message) \
    do { \
        if (_sisyphus_first_assert_passed == -1) { \
            _sisyphus_first_assert_passed = (condition) ? 1 : 0; \
        } \
        \
        if (!(condition)) { \
            /* Failure logic doesn't change */ \
            printf(TEST_COLOR_RED "💥 TRIAL FAILED: %s" TEST_COLOR_RESET, message); \
            printf(TEST_COLOR_GRAY "\n      Line %d in %s" TEST_COLOR_RESET "\n", __LINE__, __FILE__); \
            if (_sisyphus_failed_count < 10) { \
                snprintf(_sisyphus_failed_tests[_sisyphus_failed_count], 256, "%s", __func__); \
                snprintf(_sisyphus_failed_messages[_sisyphus_failed_count], 512, "%s", message); \
                snprintf(_sisyphus_failed_locations[_sisyphus_failed_count], 256, "%s:%d", __FILE__, __LINE__); \
                _sisyphus_failed_count++; \
            } \
            _sisyphus_combo_multiplier = 1; \
            return 0; \
        } else { \
            _sisyphus_trials_conquered++; \
            int xp_gain = 0; \
            \
            if (_sisyphus_in_loop) { \
                /* --- IN-LOOP LOGIC --- */ \
                if (_sisyphus_loop_iteration < 3) { \
                    printf(TEST_COLOR_GREEN "      - Iter %d: %s" TEST_COLOR_RESET, _sisyphus_loop_iteration + 1, message); \
                    printf(TEST_COLOR_YELLOW " (+1 XP)" TEST_COLOR_RESET "\n"); \
                } \
                if (_sisyphus_loop_iteration == 2) { \
                    printf(TEST_COLOR_GRAY "          Sisyphus finds a rhythm... (condensing output)\n" TEST_COLOR_RESET); \
                } \
                xp_gain = 1; /* Award a single, non-decaying XP point for loop assertions */ \
                _sisyphus_xp_from_tests += xp_gain; \
                _sisyphus_loop_iteration++; \
            } else { \
                /* --- NORMAL COMBO LOGIC (WITH CORRECTED ATTRIBUTION) --- */ \
                int base_test_xp = 5; \
                int combo_bonus = 0; \
                if (_sisyphus_combo_multiplier <= 7) { \
                    combo_bonus = 2 * (_sisyphus_combo_multiplier - 1); \
                } else { \
                    int diminish_levels = _sisyphus_combo_multiplier - 7; \
                    int diminished_bonus = 12 - (diminish_levels * 3); /* Max combo bonus is 12 */ \
                    combo_bonus = (diminished_bonus > 0) ? diminished_bonus : 0; \
                } \
                xp_gain = base_test_xp + combo_bonus; \
                _sisyphus_xp_from_tests += base_test_xp; \
                _sisyphus_xp_from_combos += combo_bonus; \
                \
                printf(TEST_COLOR_GREEN "  - COMBO x%d VICTORY: %s" TEST_COLOR_RESET, _sisyphus_combo_multiplier, message); \
                printf(TEST_COLOR_YELLOW " (+%d XP)" TEST_COLOR_RESET "\n", xp_gain); \
                _sisyphus_combo_multiplier++; \
            } \
            \
            _sisyphus_xp_earned += xp_gain; \
            if (!_sisyphus_is_full_test_mode) { \
                _check_achievements(); \
            } \
        } \
    } while(0)


#define RUN_TEST(test_func) \
do { \
        total_tests++; \
        _sisyphus_combo_multiplier = 1; \
        if (!_sisyphus_is_full_test_mode) { \
            const char* narrative_message = _get_narrative_test_message(_sisyphus_project_level); \
            printf(TEST_COLOR_CYAN "\n%s" TEST_COLOR_RESET, narrative_message); \
            printf(TEST_COLOR_GRAY " [\e]8;;file://%s/%s\e\\%s\e]8;;\e\\ - '%s']" TEST_COLOR_RESET "\n", \
                   getenv("PWD") ? getenv("PWD") : ".", __FILE__, __FILE__, #test_func); \
        } else { \
            printf(TEST_COLOR_GRAY "•"); \
        } \
        double start_time = get_time_precise(); \
        int test_result = test_func(); \
        double end_time = get_time_precise(); \
        double elapsed_time = end_time - start_time; \
        _total_test_time += elapsed_time; \
        if (elapsed_time < _sisyphus_fastest_test) _sisyphus_fastest_test = elapsed_time; \
        if (elapsed_time > _sisyphus_max_test_time) _sisyphus_max_test_time = elapsed_time; \
        if (elapsed_time < 0.000020) { \
                _sisyphus_consecutive_fast_tests++; \
            } else { \
                _sisyphus_consecutive_fast_tests = 0; \
            } \
            if (_sisyphus_first_test_failed == -1) { \
                _sisyphus_first_test_failed = test_result ? 0 : 1; \
            } \
        if (test_result) { \
            tests_passed++; \
            _sisyphus_perfect_streak++; \
            _sisyphus_current_perfect_streak++; \
            /* Track max combo using combo multiplier reached during test */ \
            int max_combo_this_test = _sisyphus_combo_multiplier - 1; \
            if (max_combo_this_test > _sisyphus_max_combo) _sisyphus_max_combo = max_combo_this_test; \
            int battle_xp = _sisyphus_trials_conquered * 2; \
            int final_xp = _sisyphus_is_full_test_mode ? (battle_xp / 4) : battle_xp; \
            _sisyphus_xp_earned += final_xp; \
            _sisyphus_xp_from_tests += final_xp; \
            printf(TEST_COLOR_GREEN "✨ TRIAL COMPLETED: %s" TEST_COLOR_RESET, #test_func); \
            printf(TEST_COLOR_BOLD_WHITE " (%.6fs)" TEST_COLOR_RESET, elapsed_time); \
            if (_sisyphus_perfect_streak >= 3) { \
                printf(TEST_COLOR_PURPLE " 🔥STREAK x%d!" TEST_COLOR_RESET, _sisyphus_perfect_streak); \
            } \
            printf("\n"); \
            if (_sisyphus_trials_conquered > 0 && !_sisyphus_is_full_test_mode) { \
                printf(TEST_COLOR_PURPLE "   🏛️  Explored %d passages! " TEST_COLOR_RESET, _sisyphus_trials_conquered); \
                printf(TEST_COLOR_YELLOW "+%d Exploration XP" TEST_COLOR_RESET "\n", final_xp); \
            } \
            if (!_sisyphus_is_full_test_mode) { \
                /* Add inner dialogue for successful tests based on story progression */ \
                const char* inner_thought = _get_inner_dialogue(_sisyphus_project_level, tests_passed); \
                printf(TEST_COLOR_GRAY "\"%s\" " TEST_COLOR_RESET "\n", inner_thought); \
                _level_up_check(); \
            } \
            /* Add newline after successful test completion */ \
            if (!_sisyphus_is_full_test_mode) { \
                printf("\n"); \
            } \
        } else { \
            tests_failed++; \
            _sisyphus_beasts_slayed += _sisyphus_trials_conquered; \
            printf(TEST_COLOR_RED "💀 SETBACK ENDURED: %s" TEST_COLOR_RESET, #test_func); \
            printf(TEST_COLOR_BOLD_WHITE " (%.6fs)" TEST_COLOR_RESET "\n", elapsed_time); \
            if (_sisyphus_trials_conquered > 0) { \
                printf(TEST_COLOR_ORANGE "   🔥 But you got %d combo%s before failing!" TEST_COLOR_RESET "\n", _sisyphus_trials_conquered, _sisyphus_trials_conquered == 1 ? "" : "s"); \
            } \
            if (!_sisyphus_is_full_test_mode) { \
                /* Add inner dialogue for failed tests based on story progression */ \
                const char* inner_thought = _get_inner_dialogue(_sisyphus_project_level, tests_passed); \
                printf(TEST_COLOR_GRAY "   \"%s\" " TEST_COLOR_RESET "You reflect on the setback, knowing the boulder must be pushed again.\n", inner_thought); \
            } \
            _sisyphus_perfect_streak = 0; \
            _sisyphus_current_perfect_streak = 0; \
            /* Add newline after failed test completion */ \
            if (!_sisyphus_is_full_test_mode) { \
                printf("\n"); \
            } \
        } \
        _sisyphus_trials_conquered = 0; \
        _sisyphus_combo_multiplier = 1; \
        /* End any suppressed combo sequence when test ends */ \
        if (_sisyphus_suppressing_combos && _sisyphus_same_message_count > 3) { \
            printf(TEST_COLOR_GREEN "   COMBO x%d VICTORY: %s" TEST_COLOR_RESET, _sisyphus_combo_multiplier - 1, _sisyphus_last_combo_message); \
            printf(TEST_COLOR_YELLOW " (+3 XP)" TEST_COLOR_RESET "\n"); \
        } \
        _sisyphus_suppressing_combos = 0; \
        _sisyphus_same_message_count = 0; \
        _sisyphus_last_combo_message[0] = '\0'; \
        if (!_sisyphus_is_full_test_mode) { \
            _check_achievements(); \
        } \
    } while(0)

// Epic Sisyphus gamified test suite header macro
#define TEST_SUITE_START(suite_name) \
    _test_suite_start_time = get_time_precise(); \
    _total_test_time = 0.0; \
    _sisyphus_beasts_slayed = 0; \
    _sisyphus_trials_conquered = 0; \
    _sisyphus_perfect_streak = 0; \
    _sisyphus_xp_earned = 0; \
    _sisyphus_combo_multiplier = 1; \
    _sisyphus_achievements_unlocked = 0; \
    _sisyphus_fastest_test = 999.0; \
    _sisyphus_max_combo = 0; \
    _sisyphus_total_expected_tests = _count_expected_tests(); \
    _sisyphus_is_full_test_mode = (getenv("SISYPHUS_FULL_TEST_MODE") != NULL); \
    _sisyphus_xp_from_tests = 0; \
    _sisyphus_xp_from_combos = 0; \
    _sisyphus_xp_from_achievements = 0; \
    _sisyphus_xp_from_failures = 0; \
    _sisyphus_unlocked_count = 0; \
    _sisyphus_consecutive_fast_tests = 0; \
    _sisyphus_current_perfect_streak = 0; \
    _sisyphus_failed_count = 0; \
    _sisyphus_current_test_penalty = 0; \
    _read_project_xp(); \
    strncpy(_sisyphus_suite_name, suite_name, sizeof(_sisyphus_suite_name) - 1); \
    if (!_sisyphus_is_full_test_mode) { \
        const char* suite_intro = _get_narrative_test_message(_sisyphus_project_level); \
        printf(TEST_COLOR_PURPLE "🏛️  LABYRINTH CONSTRUCTION: %s" TEST_COLOR_RESET "\n", suite_name); \
        printf(TEST_COLOR_CYAN "════════════════════════════════════════════════════════" TEST_COLOR_RESET "\n"); \
        printf(TEST_COLOR_GRAY "🪨 Each test pushes the boulder higher • Build the perfect maze" TEST_COLOR_RESET "\n"); \
        char total_xp_str[32], next_level_xp_str[32]; \
        format_number_with_commas(total_xp_str, sizeof(total_xp_str), _sisyphus_total_project_xp); \
        format_number_with_commas(next_level_xp_str, sizeof(next_level_xp_str), _get_xp_for_level(_sisyphus_project_level + 1)); \
        printf(TEST_COLOR_CYAN "🏗️  Architect Level: %d | Total Construction XP: %s | Next Level: %s XP" TEST_COLOR_RESET "\n", _sisyphus_project_level, total_xp_str, next_level_xp_str); \
    } else { \
        printf(TEST_COLOR_GRAY "🏗️  Workshop Session: %s" TEST_COLOR_RESET " ", suite_name); \
    }

#define TEST_SUITE_END() \
    double _suite_end_time = get_time_precise(); \
    double _total_suite_time = _suite_end_time - _test_suite_start_time; \
    if (_sisyphus_loop_iteration_count > 3) { \
            printf(TEST_COLOR_GRAY "      ... (suppressed %d similar messages)\n" TEST_COLOR_RESET, _sisyphus_loop_iteration_count - 3); \
        } \
        if (!_achievement_perfect_stride && tests_failed == 0 && total_tests > 3 && (_sisyphus_max_test_time - _sisyphus_fastest_test) < 0.000030) { \
                _achievement_perfect_stride = 1; _sisyphus_achievements_unlocked++; int bonus_xp = 50; \
                _sisyphus_xp_earned += bonus_xp; _sisyphus_xp_from_achievements += bonus_xp; \
                printf(TEST_COLOR_GREEN "🏆 ACHIEVEMENT: The Perfect Stride " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp); \
                printf(TEST_COLOR_GRAY "    For incredible consistency; every step as perfect as the last." TEST_COLOR_RESET "\n"); \
                UPDATE_BEST_ACHIEVEMENT("The Perfect Stride", bonus_xp, "Incredible consistency between all tests."); \
            } \
            \
            double overhead_time = _total_suite_time - _total_test_time; \
            if (!_achievement_hermes_caduceus && tests_failed == 0 && overhead_time < (_total_test_time * 0.5)) { \
                _achievement_hermes_caduceus = 1; _sisyphus_achievements_unlocked++; int bonus_xp = 55; \
                _sisyphus_xp_earned += bonus_xp; _sisyphus_xp_from_achievements += bonus_xp; \
                printf(TEST_COLOR_CYAN "🏆 ACHIEVEMENT: Hermes's Caduceus " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp); \
                printf(TEST_COLOR_GRAY "    A divinely efficient run where the framework overhead was minimal." TEST_COLOR_RESET "\n"); \
                UPDATE_BEST_ACHIEVEMENT("Hermes's Caduceus", bonus_xp, "A divinely efficient run."); \
            } \
            /* --- END OF ACHIEVEMENT LOGIC --- */ \
            \
            if (_sisyphus_loop_iteration_count > 3) { \
                printf(TEST_COLOR_GRAY "      ... (condensed %d subsequent messages)\n" TEST_COLOR_RESET, _sisyphus_loop_iteration_count - 3); \
            } \
            \
    /* Apply XP penalties for failures: -100, -200, -300, etc. */ \
    int xp_penalty = 0; \
    if (tests_failed > 0) { \
        for (int i = 1; i <= tests_failed; i++) { \
            xp_penalty += i * 100; \
        } \
        int original_xp = _sisyphus_xp_earned; \
        _sisyphus_xp_earned = (_sisyphus_xp_earned > xp_penalty) ? _sisyphus_xp_earned - xp_penalty : 0; \
        _sisyphus_xp_from_failures = -xp_penalty; \
        if (xp_penalty > 0) { \
            char penalty_str[32], original_str[32], final_str[32]; \
            format_number_with_commas(penalty_str, sizeof(penalty_str), xp_penalty); \
            format_number_with_commas(original_str, sizeof(original_str), original_xp); \
            format_number_with_commas(final_str, sizeof(final_str), _sisyphus_xp_earned); \
            printf(TEST_COLOR_RED "💀 FAILURE PENALTY: -%s XP (from %d failure%s)" TEST_COLOR_RESET "\n", penalty_str, tests_failed, tests_failed == 1 ? "" : "s"); \
            printf(TEST_COLOR_GRAY "   Original XP: %s → Final XP: %s" TEST_COLOR_RESET "\n", original_str, final_str); \
        } \
    } \
    _update_project_xp(); \
    _write_sisyphus_mini_stats(); \
    printf(TEST_COLOR_CYAN "════════════════════════════════════════════════════════" TEST_COLOR_RESET "\n"); \
    printf(TEST_COLOR_PURPLE "🏛️  WORKSHOP PROGRESS - LABYRINTH CONSTRUCTION REPORT" TEST_COLOR_RESET "\n"); \
    printf(TEST_COLOR_CYAN "════════════════════════════════════════════════════════" TEST_COLOR_RESET "\n"); \
    _display_progress_bar(tests_passed, total_tests, "Victory Rate", TEST_COLOR_GREEN); \
    printf(TEST_COLOR_CYAN "🧪 Maze Sections Built: %d" TEST_COLOR_RESET " | ", total_tests); \
    printf(TEST_COLOR_GREEN "🏗️  Completed: %d" TEST_COLOR_RESET " | ", tests_passed); \
    printf(TEST_COLOR_RED "🪨 Needs Rework: %d" TEST_COLOR_RESET "\n", tests_failed); \
    int current_level_xp = _get_xp_in_current_level(_sisyphus_xp_earned, _sisyphus_current_level); \
    int xp_needed_for_next = _get_xp_for_level(_sisyphus_current_level); \
    _display_progress_bar_with_level(current_level_xp, xp_needed_for_next, "Suite XP", TEST_COLOR_YELLOW, _sisyphus_current_level, 0); \
    int project_level_xp = _get_xp_in_current_level(_sisyphus_total_project_xp, _sisyphus_project_level); \
    int project_xp_needed = _get_xp_for_level(_sisyphus_project_level); \
    _display_progress_bar_with_level(project_level_xp, project_xp_needed, "Project XP", TEST_COLOR_PURPLE, _sisyphus_project_level, 1); \
    printf( "⏱️  Fastest Trial: %.6fs" TEST_COLOR_RESET " | ", _sisyphus_fastest_test < 999.0 ? _sisyphus_fastest_test : 0.0); \
    printf(TEST_COLOR_GREEN "🏆 Achievements: %d" TEST_COLOR_RESET, _sisyphus_achievements_unlocked); \
    if (_sisyphus_best_achievement_xp > 0) { \
        printf(TEST_COLOR_BOLD_WHITE " | Best Achievement: %s (%d XP)" TEST_COLOR_RESET, _sisyphus_best_achievement, _sisyphus_best_achievement_xp); \
    } \
    printf("\n"); \
    printf(TEST_COLOR_GRAY "🕒 Exploration Time: %.6fs | 🔧 Setup Time: %.6fs | 📊 Overhead: %.6fs" TEST_COLOR_RESET "\n", \
           _total_suite_time, _total_test_time, _total_suite_time - _total_test_time); \
    printf("⏱️  Total Test Time: %.6f seconds\n", _total_test_time); \
    if (tests_failed == 0) { \
        if (_sisyphus_unlocked_count > 0) { \
            int random_achievement = (_sisyphus_xp_earned + _sisyphus_max_combo + tests_passed) % _sisyphus_unlocked_count; \
            int bonus_xp = _sisyphus_unlocked_xp[random_achievement] * (_sisyphus_random_achievement_multi - 1); \
            _sisyphus_xp_earned += bonus_xp; \
            _sisyphus_xp_from_achievements += bonus_xp; \
            printf(TEST_COLOR_CYAN "\nRandom Achievement: 🏆 %s" TEST_COLOR_BOLD_WHITE " (%d XP)" TEST_COLOR_YELLOW " +%d BONUS XP!" TEST_COLOR_RESET "\n", \
                   _sisyphus_unlocked_achievements[random_achievement], _sisyphus_unlocked_xp[random_achievement], bonus_xp); \
            printf(TEST_COLOR_GRAY "   %s" TEST_COLOR_RESET "\n", _sisyphus_unlocked_descriptions[random_achievement]); \
        } \
        char total_project_xp_str[32], session_xp_str[32], test_xp_str[32], combo_xp_str[32], achievement_xp_str[32]; \
        format_number_with_commas(total_project_xp_str, sizeof(total_project_xp_str), _sisyphus_total_project_xp); \
        format_number_with_commas(session_xp_str, sizeof(session_xp_str), _sisyphus_xp_earned); \
        format_number_with_commas(test_xp_str, sizeof(test_xp_str), _sisyphus_xp_from_tests); \
        format_number_with_commas(combo_xp_str, sizeof(combo_xp_str), _sisyphus_xp_from_combos); \
        format_number_with_commas(achievement_xp_str, sizeof(achievement_xp_str), _sisyphus_xp_from_achievements); \
        printf(TEST_COLOR_BOLD_WHITE "🏛️  Project Total XP: %s" TEST_COLOR_RESET " | ", total_project_xp_str); \
        printf(TEST_COLOR_BOLD_WHITE "🌟 Project Level: %d" TEST_COLOR_RESET " | ", _sisyphus_project_level); \
        printf(TEST_COLOR_GRAY "📈 XP Gained This Session: +%s" TEST_COLOR_RESET "\n", session_xp_str); \
        printf(TEST_COLOR_CYAN "💰 XP from tests: %s, " TEST_COLOR_RED "XP from combos: %s," TEST_COLOR_YELLOW " XP from achievements: %s" TEST_COLOR_RESET "\n", test_xp_str, combo_xp_str, achievement_xp_str); \
        return 0; \
    } else { \
        if (_sisyphus_failed_count > 0) { \
            printf(TEST_COLOR_RED "\n════════════════════════════════════════════════════════" TEST_COLOR_RESET "\n"); \
            printf(TEST_COLOR_RED "💀 BATTLE CASUALTIES - FAILED TRIALS" TEST_COLOR_RESET "\n"); \
            printf(TEST_COLOR_RED "════════════════════════════════════════════════════════" TEST_COLOR_RESET "\n"); \
            for (int i = 0; i < _sisyphus_failed_count; i++) { \
                printf(TEST_COLOR_RED "🔴 %s: %s" TEST_COLOR_RESET "\n", _sisyphus_failed_tests[i], _sisyphus_failed_messages[i]); \
                printf(TEST_COLOR_GRAY "   📍 %s" TEST_COLOR_RESET "\n", _sisyphus_failed_locations[i]); \
            } \
            printf(TEST_COLOR_RED "════════════════════════════════════════════════════════" TEST_COLOR_RESET "\n"); \
        } \
        printf(TEST_COLOR_ORANGE "\n🪨 THE BOULDER SLIPS! But Sisyphus always pushes again!" TEST_COLOR_RESET "\n"); \
        char level_xp_str[32]; \
        format_number_with_commas(level_xp_str, sizeof(level_xp_str), _sisyphus_xp_earned); \
        printf(TEST_COLOR_YELLOW "🏗️ Construction Level %d achieved with %s XP - The work continues!" TEST_COLOR_RESET "\n", _sisyphus_current_level, level_xp_str); \
        /* Display random motivational quote */ \
        int quote_index = (_sisyphus_xp_earned + tests_failed + tests_passed) % _motivational_quotes_count; \
        printf(TEST_COLOR_GRAY "📈 %s" TEST_COLOR_RESET "\n", _motivational_quotes[quote_index]); \
        if (_sisyphus_unlocked_count > 0) { \
            int random_achievement = (_sisyphus_xp_earned + _sisyphus_max_combo + tests_passed) % _sisyphus_unlocked_count; \
            int bonus_xp = _sisyphus_unlocked_xp[random_achievement] * (_sisyphus_random_achievement_multi - 1); \
            _sisyphus_xp_earned += bonus_xp; \
            _sisyphus_xp_from_achievements += bonus_xp; \
            printf(TEST_COLOR_CYAN "\nRandom Achievement: 🏆 %s" TEST_COLOR_BOLD_WHITE " (%d XP)" TEST_COLOR_YELLOW " +%d BONUS XP!" TEST_COLOR_RESET "\n", \
                   _sisyphus_unlocked_achievements[random_achievement], _sisyphus_unlocked_xp[random_achievement], bonus_xp); \
            printf(TEST_COLOR_GRAY "   %s" TEST_COLOR_RESET "\n", _sisyphus_unlocked_descriptions[random_achievement]); \
        } \
        char total_project_xp_str2[32], session_xp_str2[32], test_xp_str2[32], combo_xp_str2[32], achievement_xp_str2[32]; \
        format_number_with_commas(total_project_xp_str2, sizeof(total_project_xp_str2), _sisyphus_total_project_xp); \
        format_number_with_commas(session_xp_str2, sizeof(session_xp_str2), _sisyphus_xp_earned); \
        format_number_with_commas(test_xp_str2, sizeof(test_xp_str2), _sisyphus_xp_from_tests); \
        format_number_with_commas(combo_xp_str2, sizeof(combo_xp_str2), _sisyphus_xp_from_combos); \
        format_number_with_commas(achievement_xp_str2, sizeof(achievement_xp_str2), _sisyphus_xp_from_achievements); \
        printf(TEST_COLOR_BOLD_WHITE "🏛️  Project Total XP: %s" TEST_COLOR_RESET " | ", total_project_xp_str2); \
        printf(TEST_COLOR_BOLD_WHITE "🌟 Project Level: %d" TEST_COLOR_RESET " | ", _sisyphus_project_level); \
        printf(TEST_COLOR_GRAY "📈 XP Gained This Session: +%s" TEST_COLOR_RESET "\n", session_xp_str2); \
        printf(TEST_COLOR_CYAN "💰 XP from tests: %s, " TEST_COLOR_RED "XP from combos: %s," TEST_COLOR_YELLOW " XP from achievements: %s" TEST_COLOR_RESET "\n", test_xp_str2, combo_xp_str2, achievement_xp_str2); \
        return 1; \
    }

// Compact gamified version for when running multiple test files in the great test runner
#define TEST_SUITE_END_QUIET() \
    double _suite_end_time = get_time_precise(); \
    double _total_suite_time = _suite_end_time - _test_suite_start_time; \
    _update_project_xp(); \
    _write_sisyphus_mini_stats(); \
    printf(TEST_COLOR_BOLD_WHITE "⚔️  Trials: %d" TEST_COLOR_RESET " | ", total_tests); \
    printf(TEST_COLOR_GREEN "🏆 Won: %d" TEST_COLOR_RESET " | ", tests_passed); \
    printf(TEST_COLOR_RED "💀 Lost: %d" TEST_COLOR_RESET " | ", tests_failed); \
    printf(TEST_COLOR_YELLOW "💰 +%dXP" TEST_COLOR_RESET " | ", _sisyphus_xp_earned); \
    printf(TEST_COLOR_PURPLE "Lv.%d" TEST_COLOR_RESET " | ", _sisyphus_current_level); \
    printf(TEST_COLOR_CYAN "🔥 %dx" TEST_COLOR_RESET " | ", _sisyphus_max_combo); \
    printf(TEST_COLOR_BOLD_WHITE "🌟 ProjLv.%d" TEST_COLOR_RESET "\n", _sisyphus_project_level); \
    printf(TEST_COLOR_GRAY "⏱️  Exploration: %.6fs | 🏆 %d achievements | ✨ %.6fs fastest" TEST_COLOR_RESET "\n", \
           _total_test_time, _sisyphus_achievements_unlocked, _sisyphus_fastest_test < 999.0 ? _sisyphus_fastest_test : 0.0); \
    printf("⏱️  Total Test Time: %.6f seconds\n", _total_test_time); \
    char quiet_test_xp_str[32], quiet_combo_xp_str[32], quiet_achievement_xp_str[32], quiet_failure_xp_str[32]; \
    format_number_with_commas(quiet_test_xp_str, sizeof(quiet_test_xp_str), _sisyphus_xp_from_tests); \
    format_number_with_commas(quiet_combo_xp_str, sizeof(quiet_combo_xp_str), _sisyphus_xp_from_combos); \
    format_number_with_commas(quiet_achievement_xp_str, sizeof(quiet_achievement_xp_str), _sisyphus_xp_from_achievements); \
    format_number_with_commas(quiet_failure_xp_str, sizeof(quiet_failure_xp_str), _sisyphus_xp_from_failures); \
    printf(TEST_COLOR_CYAN "💰 XP from tests: %s, " TEST_COLOR_ORANGE "XP from combos: %s, " TEST_COLOR_YELLOW "XP from achievements: %s", quiet_test_xp_str, quiet_combo_xp_str, quiet_achievement_xp_str); \
    if (_sisyphus_xp_from_failures != 0) { \
        printf(TEST_COLOR_RED ", XP from failures: %s" TEST_COLOR_RESET "\n", quiet_failure_xp_str); \
    } else { \
        printf(TEST_COLOR_RESET "\n"); \
    } \
    if (tests_failed == 0) { \
        char perfect_xp_str[32]; \
        format_number_with_commas(perfect_xp_str, sizeof(perfect_xp_str), _sisyphus_xp_earned); \
        printf(TEST_COLOR_PURPLE "✨ Flawless construction! +%s XP added to Labyrinth Level %d!" TEST_COLOR_RESET "\n", perfect_xp_str, _sisyphus_project_level); \
        return 0; \
    } else { \
        char gained_xp_str[32]; \
        format_number_with_commas(gained_xp_str, sizeof(gained_xp_str), _sisyphus_xp_earned); \
        printf(TEST_COLOR_ORANGE "🪨 +%s XP earned! Labyrinth Level %d progress preserved!" TEST_COLOR_RESET "\n", gained_xp_str, _sisyphus_project_level); \
        return 1; \
    }

#endif
