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

// Achievement flags
static int _achievement_first_blood = 0;
static int _achievement_combo_master = 0;
static int _achievement_speed_demon = 0;
static int _achievement_perfectionist = 0;
// Additional achievement flags
static int _achievement_lightning_fast = 0;      // Complete all tests under 1ms total
static int _achievement_atlas_endurance = 0;    // Complete 50+ tests in one suite
static int _achievement_hermes_messenger = 0;   // Complete suite in under 0.001 seconds
static int _achievement_hydra_slayer = 0;       // Overcome failures and still win
static int _achievement_midas_touch = 0;        // Get 100+ XP in single suite
static int _achievement_odyssey_complete = 0;   // Complete suite with perfect accuracy
static int _achievement_spartan_warrior = 0;   // 500+ XP, 8+ combo, zero defeats (elite warrior!)
static int _achievement_trojan_horse = 0;       // Unexpected comeback (fail first, then all pass)
static int _achievement_phoenix_rising = 0;     // Achieve max combo after breaking streak
static int _achievement_golden_fleece = 0;      // Find the fastest possible time (under 10µs)
static int _achievement_pandoras_box = 0;       // Unlock 5+ achievements in one run
static int _achievement_oracle_wisdom = 0;     // 7+ achievements + lightning speed
static int _achievement_titan_strength = 0;     // Earn 1000+ total project XP
static int _achievement_nectar_gods = 0;        // Perfect run with 10x combo multiplier
static int _achievement_sisyphus_persistence = 0; // Come back from defeat to victory
// New easy-medium achievements
static int _achievement_apprentice_coder = 0;    // Pass 5 tests in a row
static int _achievement_steady_hands = 0;        // Complete 3 test functions
static int _achievement_code_warrior = 0;        // Earn 100+ XP in single suite
// New ultra-hard achievement
static int _achievement_divine_perfection = 0;   // 50+ tests, 0 failures, sub-100μs average
// New medium-hard historical achievements
static int _achievement_marathon_runner = 0;     // 26+ tests passed (Marathon distance)
static int _achievement_olympic_champion = 0;    // Win with 4+ different achievement types
// New unique sought-after achievements
static int _achievement_alchemist_precision = 0; // Exactly 13 perfect tests + speed
static int _achievement_archimedes_eureka = 0;   // 12+ combo + sub-1ms + 15+ tests
static int _achievement_philosopher_king = 0;    // 10+ achievements + 25+ perfect + 15+ combo
// More achievable overtaking achievements
static int _achievement_athena_strategy = 0;     // Perfect first 10 tests (40 XP)
static int _achievement_apollo_harmony = 0;     // Balance of speed and accuracy (35 XP)
static int _achievement_hermes_swiftness = 0;   // Complete in under 0.0003s total (38 XP)
// New educational achievements with higher XP values
static int _achievement_socratic_method = 0;     // Learn from failure then achieve perfection (45 XP)
static int _achievement_alexander_conquest = 0;  // Rapid domination with high XP (50 XP)
static int _achievement_euclidean_proof = 0;     // Mathematical precision and systematic approach (42 XP)
// Ultra Combo Achievements
static int _achievement_sisyphus_mastery = 0;     // 100+ combo streak
static int _achievement_prometheus_fire = 0;     // 150+ combo streak
static int _achievement_kraken_unleashed = 0;    // 200+ combo streak
static int _achievement_olympus_ascended = 0;    // 250+ combo streak

// Speed Demon Upgrades
static int _achievement_hermes_wings = 0;        // Sub-20 microseconds
static int _achievement_lightning_bolt = 0;      // Sub-15 microseconds
static int _achievement_time_lord = 0;           // Sub-10 microseconds

// Inventory-Specific Achievements
static int _achievement_hoarder_dragon = 0;      // 1000+ inventory operations
static int _achievement_master_organizer = 0;    // Perfect inventory management
static int _achievement_swift_merchant = 0;      // Rapid operations

// Endurance/Persistence Achievements
static int _achievement_marathon_god = 0;        // 500+ tests in one run
static int _achievement_eternal_vigilance = 0;   // 1000+ tests in one run
static int _achievement_unstoppable_force = 0;   // 50+ test functions

// XP Milestone Achievements
static int _achievement_treasure_hunter = 0;     // 10,000+ XP in one suite
static int _achievement_gold_rush = 0;           // 5,000+ XP in under 1ms
static int _achievement_croesus_wealth = 0;      // 200,000+ total project XP

// Meta Achievements
static int _achievement_achievement_hunter = 0;   // Unlock 25+ achievements
static int _achievement_trophy_collector = 0;    // Unlock 50+ achievements
static int _achievement_pantheon_member = 0;     // Unlock all base achievements

// Streak-Based Achievements
static int _achievement_flawless_victory = 0;    // 15+ perfect test streak
static int _achievement_domination = 0;          // 30+ perfect test streak
static int _achievement_godlike = 0;             // 100+ perfect test streak


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

static void _check_achievements(void) {
    // Skip achievements during full test mode (make test run)
    if (_sisyphus_is_full_test_mode) {
        return;
    }

    // Reduce XP rewards if there are test failures
    int xp_multiplier = (tests_failed > 0) ? 1 : 2;

    // HISTORICAL XP VALUES - meaningful numbers from ancient history
    // Basic achievements for encouragement (lower XP rewards)

    if (!_achievement_first_blood && _sisyphus_max_combo >= 7) {
        _achievement_first_blood = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 7 / xp_multiplier; // Seven against Thebes
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_GREEN "🏆 ACHIEVEMENT UNLOCKED: First Blood! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   First combo streak (x7) achieved - the taste of victory! (Seven against Thebes)" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("First Blood", bonus_xp, "First combo streak (x7) achieved - the taste of victory! (Seven against Thebes)");
    }

    if (!_achievement_speed_demon && _sisyphus_fastest_test < 0.000050) {
        _achievement_speed_demon = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 12 / xp_multiplier; // Twelve Labors of Hercules
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_CYAN "🏆 ACHIEVEMENT UNLOCKED: Speed Demon! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   Completed a trial in under 50 microseconds (Twelve Labors pace)" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Speed Demon", bonus_xp, "Completed a trial in under 50 microseconds (Twelve Labors pace)");
    }

    if (!_achievement_midas_touch && _sisyphus_xp_from_achievements >= (_sisyphus_xp_earned * 0.6) && _sisyphus_xp_earned >= 200) {
        _achievement_midas_touch = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 24 / xp_multiplier; // 24 books of Homer's epics
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_YELLOW "🏆 ACHIEVEMENT UNLOCKED: 👑 Midas Touch! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   60%% XP from achievements - everything turns to gold! (Homer's 24 books)" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Midas Touch", bonus_xp, "60%+ XP from achievements - everything turns to gold! (Homer's 24 books)");
    }

    if (!_achievement_hermes_messenger && _sisyphus_consecutive_fast_tests >= 3) {
        _achievement_hermes_messenger = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 3 / xp_multiplier; // Three Fates
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_BLUE "🏆 ACHIEVEMENT UNLOCKED: 🏃 Hermes Messenger! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   3+ consecutive lightning tests - divine messenger speed! (Three Fates)" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Hermes Messenger", bonus_xp, "3+ consecutive lightning tests - divine messenger speed! (Three Fates)");
    }

    if (!_achievement_golden_fleece && _sisyphus_fastest_test < 0.000001) {
        _achievement_golden_fleece = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 25 / xp_multiplier; // 25 Argonauts (reduced from 50)
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_YELLOW "🏆 ACHIEVEMENT UNLOCKED: 🐏 Golden Fleece! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   Found the impossible speed - %.6fs is legendary! (25 Argonauts)" TEST_COLOR_RESET "\n", _sisyphus_fastest_test);
        UPDATE_BEST_ACHIEVEMENT("Golden Fleece", bonus_xp, "Found the impossible speed - legendary! (25 Argonauts)");
    }

    // Moderate achievements requiring more effort

    if (!_achievement_lightning_fast && _total_test_time < 0.0005 && _sisyphus_trials_conquered >= 8) {
        _achievement_lightning_fast = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 9 / xp_multiplier; // Nine Muses
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_CYAN "🏆 ACHIEVEMENT UNLOCKED: ✨ Zeus's Lightning! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   Completed %d battles in under 0.5ms - divine speed! (Nine Muses)" TEST_COLOR_RESET "\n", _sisyphus_trials_conquered);
        UPDATE_BEST_ACHIEVEMENT("Zeus's Lightning", bonus_xp, "Completed battles in under 0.5ms - divine speed! (Nine Muses)");
    }

    if (!_achievement_spartan_warrior && _sisyphus_xp_earned >= 500 && _sisyphus_max_combo >= 10 && tests_failed == 0) {
        _achievement_spartan_warrior = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 30 / xp_multiplier;
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_RED "🏆 ACHIEVEMENT UNLOCKED: ⚔️ Spartan Warrior! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   500+ XP, 10+ combo, zero defeats - This is Sparta! (300 warriors)" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Spartan Warrior", bonus_xp, "500+ XP, 10+ combo, zero defeats - This is Sparta! (300 warriors)");
    }

    // High-tier achievements for mastery (highest XP but very rare)

    if (!_achievement_combo_master && _sisyphus_max_combo >= 20) {
        _achievement_combo_master = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 21 / xp_multiplier; // Reduced from 42 - still meaningful
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_PURPLE "🏆 ACHIEVEMENT UNLOCKED: Combo Master! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   Achieved a 20+ perfect trial streak with diminishing returns!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Combo Master", bonus_xp, "Achieved a 20+ perfect trial streak with diminishing returns!");
    }

    if (!_achievement_perfectionist && tests_passed >= 30 && tests_failed == 0) {
        _achievement_perfectionist = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 18 / xp_multiplier; // Reduced from 36 - more balanced
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_GREEN "🏆 ACHIEVEMENT UNLOCKED: Perfectionist! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   Completed 30+ trials without failure (Perfect circle)" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Perfectionist", bonus_xp, "Completed 30+ trials without failure (Perfect circle)");
    }

    // New achievement implementations with contextual requirements

    if (!_achievement_hydra_slayer && tests_failed >= 3 && tests_passed >= tests_failed * 2) {
        _achievement_hydra_slayer = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 9 / xp_multiplier; // Hydra's nine heads
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_ORANGE "🏆 ACHIEVEMENT UNLOCKED: 🐍 Hydra Slayer! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   Overcame %d failures and still triumphed (Nine heads conquered)" TEST_COLOR_RESET "\n", tests_failed);
        UPDATE_BEST_ACHIEVEMENT("Hydra Slayer", bonus_xp, "Overcame failures and still triumphed (Nine heads conquered)");
    }

    if (!_achievement_odyssey_complete && tests_passed >= 20 && tests_failed == 0 && _sisyphus_fastest_test < 0.0001) {
        _achievement_odyssey_complete = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 10 / xp_multiplier; // Reduced from 20 - more balanced
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_BLUE "🏆 ACHIEVEMENT UNLOCKED: 🚢 Odyssey Complete! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   Perfect accuracy + lightning speed (Epic journey)" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Odyssey Complete", bonus_xp, "Perfect accuracy + lightning speed (Epic journey)");
    }

    if (!_achievement_trojan_horse && tests_failed >= 1 && _sisyphus_perfect_streak >= 10) {
        _achievement_trojan_horse = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 10 / xp_multiplier; // 10 years of Troy siege
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_CYAN "🏆 ACHIEVEMENT UNLOCKED: 🐴 Trojan Horse! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   Unexpected comeback - failed then conquered all (10-year siege)" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Trojan Horse", bonus_xp, "Unexpected comeback - failed then conquered all (10-year siege)");
    }

    if (!_achievement_phoenix_rising && tests_failed >= 1 && _sisyphus_current_perfect_streak >= 8) {
        _achievement_phoenix_rising = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 5 / xp_multiplier; // Much more reasonable - phoenix feathers
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_PURPLE "🏆 ACHIEVEMENT UNLOCKED: 🔥 Phoenix Rising! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   Rose from failure to 8+ perfect streak - reborn from ashes! (Phoenix feathers)" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Phoenix Rising", bonus_xp, "Rose from failure to 8+ perfect streak - reborn from ashes! (Phoenix feathers)");
    }

    if (!_achievement_pandoras_box && _sisyphus_achievements_unlocked >= 5) {
        _achievement_pandoras_box = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 1; // Hope was the last thing in the box - already minimal
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_YELLOW "🏆 ACHIEVEMENT UNLOCKED: 📦 Pandora's Box! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   Unlocked 5+ achievements - Hope remains! (Last in the box)" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Pandora's Box", bonus_xp, "Unlocked 5+ achievements - Hope remains! (Last in the box)");
    }

    if (!_achievement_oracle_wisdom && _sisyphus_achievements_unlocked >= 7 && _sisyphus_fastest_test < 0.00005) {
        _achievement_oracle_wisdom = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 7 / xp_multiplier; // Seven Sages of Greece
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_CYAN "🏆 ACHIEVEMENT UNLOCKED: 🔮 Oracle Wisdom! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   Seven achievements + lightning prophecy - divine foresight!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Oracle Wisdom", bonus_xp, "Seven achievements + lightning prophecy - divine foresight!");
    }

    if (!_achievement_atlas_endurance && _sisyphus_trials_conquered >= 50) {
        _achievement_atlas_endurance = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 12 / xp_multiplier; // Twelve Titans
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_PURPLE "🏆 ACHIEVEMENT UNLOCKED: 💪 Atlas Endurance! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   Shouldered %d battles - titan strength! (Twelve Titans)" TEST_COLOR_RESET "\n", _sisyphus_trials_conquered);
        UPDATE_BEST_ACHIEVEMENT("Atlas Endurance", bonus_xp, "Shouldered battles - titan strength! (Twelve Titans)");
    }

    if (!_achievement_nectar_gods && _sisyphus_max_combo >= 12 && _sisyphus_trials_conquered >= 10) {
        _achievement_nectar_gods = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 12 / xp_multiplier; // Twelve Olympians
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_GOLD "🏆 ACHIEVEMENT UNLOCKED: 🍯 Nectar of Gods! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   Perfect run with 12+ combo (Twelve Olympians feast)" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Nectar of Gods", bonus_xp, "Perfect run with 12+ combo (Twelve Olympians feast)");
    }

    if (!_achievement_titan_strength && _sisyphus_xp_from_tests >= 1000) {
        _achievement_titan_strength = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 60 / xp_multiplier; // Six original Titans
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_PURPLE "🏆 ACHIEVEMENT UNLOCKED: ⛰️ Titan Strength! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   1000+ test XP earned - primordial power! (Six Titans)" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Titan Strength", bonus_xp, "1000+ test XP earned - primordial power! (Six Titans)");
    }

    if (!_achievement_sisyphus_persistence && tests_failed >= 5 && tests_passed >= 20) {
        _achievement_sisyphus_persistence = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 1; // Eternal struggle, humble reward - already minimal
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_GRAY "🏆 ACHIEVEMENT UNLOCKED: 🪨 Sisyphus Persistence! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   Came back from defeat to victory - the eternal struggle!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Sisyphus Persistence", bonus_xp, "Came back from defeat to victory - the eternal struggle!");
    }

    // NEW EASY-MEDIUM ACHIEVEMENTS

    if (!_achievement_apprentice_coder && _sisyphus_max_combo >= 5) {
        _achievement_apprentice_coder = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 5 / xp_multiplier; // Simple milestone
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_GREEN "🏆 ACHIEVEMENT UNLOCKED: 🎓 Apprentice Coder! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   Achieved 5x combo streak - learning the ropes!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Apprentice Coder", bonus_xp, "Achieved 5x combo streak - learning the ropes!");
    }

    if (!_achievement_steady_hands && tests_passed >= 3) {
        _achievement_steady_hands = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 3 / xp_multiplier; // Three test functions completed
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_CYAN "🏆 ACHIEVEMENT UNLOCKED: ✋ Steady Hands! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   Completed 3 test functions - building consistency!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Steady Hands", bonus_xp, "Completed 3 test functions - building consistency!");
    }

    if (!_achievement_code_warrior && _sisyphus_xp_earned >= 100) {
        _achievement_code_warrior = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 15 / xp_multiplier; // Moderate achievement
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_ORANGE "🏆 ACHIEVEMENT UNLOCKED: ⚔️ Code Warrior! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   Earned 100+ XP in single suite - battle-tested!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Code Warrior", bonus_xp, "Earned 100+ XP in single suite - battle-tested!");
    }

    // ULTRA-HARD ACHIEVEMENT (100+ XP)

    if (!_achievement_divine_perfection && tests_passed >= 50 && tests_failed == 0 && _total_test_time < 0.005) {
        _achievement_divine_perfection = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 144 / xp_multiplier; // 12 squared - perfect divine number
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_GOLD "🏆 ACHIEVEMENT UNLOCKED: ✨ DIVINE PERFECTION! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_BOLD_WHITE "   🌟 50+ tests, ZERO failures, sub-5ms total time - GODLIKE! 🌟" TEST_COLOR_RESET "\n");
        printf(TEST_COLOR_GRAY "   You have transcended mortal coding limitations!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Divine Perfection", bonus_xp, "50+ tests, ZERO failures, sub-5ms total time - GODLIKE!");
    }

    // NEW MEDIUM-HARD HISTORICAL ACHIEVEMENTS

    if (!_achievement_marathon_runner && tests_passed >= 26 && tests_failed <= 2) {
        _achievement_marathon_runner = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 26 / xp_multiplier; // Marathon distance: 26.2 miles from Marathon to Athens
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_BLUE "🏆 ACHIEVEMENT UNLOCKED: 🏃 Marathon Runner! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   Ran 26+ tests like Pheidippides' legendary run to Athens!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Marathon Runner", bonus_xp, "Ran 26+ tests like Pheidippides' legendary run to Athens!");
    }

    if (!_achievement_olympic_champion && _sisyphus_achievements_unlocked >= 4 && _sisyphus_max_combo >= 10) {
        _achievement_olympic_champion = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 32 / xp_multiplier;
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_PURPLE "🏆 ACHIEVEMENT UNLOCKED: 🥇 Olympic Champion! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   4+ achievements + 10+ combo - worthy of Olympian laurels!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Olympic Champion", bonus_xp, "4+ achievements + 10+ combo - worthy of Olympian laurels!");
    }

    // NEW UNIQUE SOUGHT-AFTER ACHIEVEMENTS

    if (!_achievement_alchemist_precision && tests_passed == 13 && tests_failed == 0 && _sisyphus_fastest_test < 0.0001) {
        _achievement_alchemist_precision = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 33 / xp_multiplier; // Master number in alchemy/numerology
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_PURPLE "🏆 ACHIEVEMENT UNLOCKED: 🧪 Alchemist Precision! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   Exactly 13 perfect tests - transmuted bad luck to gold!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Alchemist Precision", bonus_xp, "Exactly 13 perfect tests - transmuted bad luck to gold!");
    }

    if (!_achievement_archimedes_eureka && _sisyphus_max_combo >= 12 && _total_test_time < 0.001 && tests_passed >= 15) {
        _achievement_archimedes_eureka = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 42 / xp_multiplier; // Answer to everything (Douglas Adams tribute)
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_YELLOW "🏆 ACHIEVEMENT UNLOCKED: 💡 Archimedes Eureka! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   'I have found it!' - Perfect speed + combo mastery!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Archimedes Eureka", bonus_xp, "'I have found it!' - Perfect speed + combo mastery!");
    }

    if (!_achievement_philosopher_king && _sisyphus_achievements_unlocked >= 10 && tests_passed >= 25 && tests_failed == 0 && _sisyphus_max_combo >= 20) {
        _achievement_philosopher_king = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 99 / xp_multiplier; // Almost divine perfection (99% wisdom)
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_GOLD "🏆 ACHIEVEMENT UNLOCKED: 👑 Philosopher King! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_BOLD_WHITE "   🎓 Plato's ideal ruler: wisdom, power, and perfect justice! 🎓" TEST_COLOR_RESET "\n");
        printf(TEST_COLOR_GRAY "   10+ achievements, 25+ perfect tests, 20+ combo - true mastery!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Philosopher King", bonus_xp, "Plato's ideal ruler: wisdom, power, and perfect justice!");
    }

    // MORE ACHIEVABLE OVERTAKING ACHIEVEMENTS

    if (!_achievement_athena_strategy && tests_passed >= 10 && tests_failed == 0 && _sisyphus_trials_conquered >= 10) {
        _achievement_athena_strategy = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 40 / xp_multiplier; // Wisdom goddess strategic bonus
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_BLUE "🏆 ACHIEVEMENT UNLOCKED: 🦉 Athena Strategy! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   Perfect first 10 tests - wise strategic planning!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Athena Strategy", bonus_xp, "Perfect first 10 tests - wise strategic planning!");
    }

    if (!_achievement_apollo_harmony && _sisyphus_max_combo >= 10 && _sisyphus_fastest_test < 0.00003 && tests_passed >= 12) {
        _achievement_apollo_harmony = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 35 / xp_multiplier; // God of harmony and perfection
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_YELLOW "🏆 ACHIEVEMENT UNLOCKED: 🎵 Apollo Harmony! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   Perfect balance of speed and combo mastery!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Apollo Harmony", bonus_xp, "Perfect balance of speed and combo mastery!");
    }

    if (!_achievement_hermes_swiftness && _total_test_time < 0.0003 && tests_passed >= 9) {
        _achievement_hermes_swiftness = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 38 / xp_multiplier; // Messenger god speed bonus
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_CYAN "🏆 ACHIEVEMENT UNLOCKED: 🚀 Hermes Swiftness! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   Sub-0.3ms total time - divine messenger speed!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Hermes Swiftness", bonus_xp, "Sub-0.3ms total time - divine messenger speed!");
    }

    // Ultra Combo Achievements
    if (!_achievement_sisyphus_mastery && _sisyphus_max_combo >= 100) {
        _achievement_sisyphus_mastery = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 100 / xp_multiplier; // Century milestone
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_PURPLE "🏆 ACHIEVEMENT UNLOCKED: 🗿 Sisyphus Mastery! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   100+ combo streak - you've mastered the eternal struggle!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Sisyphus Mastery", bonus_xp, "100+ combo streak - mastered the eternal struggle!");
    }

    if (!_achievement_prometheus_fire && _sisyphus_max_combo >= 150) {
        _achievement_prometheus_fire = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 150 / xp_multiplier; // Fire of knowledge
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_ORANGE "🏆 ACHIEVEMENT UNLOCKED: 🔥 Prometheus Fire! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   150+ combo streak - stolen fire from the gods!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Prometheus Fire", bonus_xp, "150+ combo streak - stolen fire from the gods!");
    }

    if (!_achievement_kraken_unleashed && _sisyphus_max_combo >= 200) {
        _achievement_kraken_unleashed = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 200 / xp_multiplier; // Beast of the deep
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_BLUE "🏆 ACHIEVEMENT UNLOCKED: 🐙 Kraken Unleashed! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   200+ combo streak - unleashed the beast of the deep!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Kraken Unleashed", bonus_xp, "200+ combo streak - unleashed the beast of the deep!");
    }

    if (!_achievement_olympus_ascended && _sisyphus_max_combo >= 250) {
        _achievement_olympus_ascended = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 250 / xp_multiplier; // Divine ascension
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_GOLD "🏆 ACHIEVEMENT UNLOCKED: ⛰️ Olympus Ascended! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   250+ combo streak - climbed to the realm of gods!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Olympus Ascended", bonus_xp, "250+ combo streak - climbed to the realm of gods!");
    }

    // Speed Achievements
    if (!_achievement_hermes_wings && _sisyphus_fastest_test < 0.000020) {
        _achievement_hermes_wings = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 20 / xp_multiplier; // Sub-20 microseconds
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_CYAN "🏆 ACHIEVEMENT UNLOCKED: 🪶 Hermes Wings! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   Sub-20μs test - winged feet of the messenger god!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Hermes Wings", bonus_xp, "Sub-20μs test - winged feet of the messenger god!");
    }

    if (!_achievement_lightning_bolt && _sisyphus_fastest_test < 0.000015) {
        _achievement_lightning_bolt = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 15 / xp_multiplier; // Sub-15 microseconds
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_YELLOW "🏆 ACHIEVEMENT UNLOCKED: ⚡ Lightning Bolt! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   Sub-15μs test - faster than Zeus's lightning!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Lightning Bolt", bonus_xp, "Sub-15μs test - faster than Zeus's lightning!");
    }

    if (!_achievement_time_lord && _sisyphus_fastest_test < 0.000010) {
        _achievement_time_lord = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 42 / xp_multiplier; // Answer to everything
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_PURPLE "🏆 ACHIEVEMENT UNLOCKED: ⏰ Time Lord! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   Sub-10μs test - master of time itself!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Time Lord", bonus_xp, "Sub-10μs test - master of time itself!");
    }

    // XP Achievements
    if (!_achievement_treasure_hunter && _sisyphus_xp_earned >= 10000) {
        _achievement_treasure_hunter = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 100 / xp_multiplier; // Treasure milestone
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_YELLOW "🏆 ACHIEVEMENT UNLOCKED: 💰 Treasure Hunter! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   10,000+ XP in one suite - found the mother lode!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Treasure Hunter", bonus_xp, "10,000+ XP in one suite - found the mother lode!");
    }

    if (!_achievement_gold_rush && _sisyphus_xp_earned >= 5000 && _total_test_time < 0.001) {
        _achievement_gold_rush = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 49 / xp_multiplier; // California Gold Rush
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_GOLD "🏆 ACHIEVEMENT UNLOCKED: 🏃‍♂️ Gold Rush! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   5,000+ XP in under 1ms - struck gold fast!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Gold Rush", bonus_xp, "5,000+ XP in under 1ms - struck gold fast!");
    }

    // Test Count Achievements
    if (!_achievement_marathon_god && _sisyphus_trials_conquered >= 500) {
        _achievement_marathon_god = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 80 / xp_multiplier; // Marathon distance
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_GREEN "🏆 ACHIEVEMENT UNLOCKED: 🏃 Marathon God! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   500+ tests in one run - divine endurance!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Marathon God", bonus_xp, "500+ tests in one run - divine endurance!");
    }

    if (!_achievement_eternal_vigilance && _sisyphus_trials_conquered >= 1000) {
        _achievement_eternal_vigilance = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 150 / xp_multiplier; // Eternal vigilance
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_PURPLE "🏆 ACHIEVEMENT UNLOCKED: 👁️ Eternal Vigilance! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   1000+ tests in one run - eternal watchfulness!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Eternal Vigilance", bonus_xp, "1000+ tests in one run - eternal watchfulness!");
    }

    // Meta Achievements
    if (!_achievement_achievement_hunter && _sisyphus_achievements_unlocked >= 25) {
        _achievement_achievement_hunter = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 25 / xp_multiplier; // Achievement milestone
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_CYAN "🏆 ACHIEVEMENT UNLOCKED: 🎯 Achievement Hunter! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   Unlocked 25+ achievements - dedicated collector!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Achievement Hunter", bonus_xp, "Unlocked 25+ achievements - dedicated collector!");
    }

    // Streak Achievements
    if (!_achievement_flawless_victory && _sisyphus_perfect_streak >= 15) {
        _achievement_flawless_victory = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 15 / xp_multiplier; // Perfect streak
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_GREEN "🏆 ACHIEVEMENT UNLOCKED: 🎯 Flawless Victory! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   15+ perfect test streak - flawless execution!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Flawless Victory", bonus_xp, "15+ perfect test streak - flawless execution!");
    }

    if (!_achievement_domination && _sisyphus_perfect_streak >= 30) {
        _achievement_domination = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 30 / xp_multiplier; // Domination
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_RED "🏆 ACHIEVEMENT UNLOCKED: 👑 Domination! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   30+ perfect test streak - total domination!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Domination", bonus_xp, "30+ perfect test streak - total domination!");
    }

    if (!_achievement_godlike && _sisyphus_perfect_streak >= 100) {
        _achievement_godlike = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 100 / xp_multiplier; // Godlike performance
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_GOLD "🏆 ACHIEVEMENT UNLOCKED: ⚡ Godlike! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   100+ perfect test streak - ascended to godhood!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Godlike", bonus_xp, "100+ perfect test streak - ascended to godhood!");
    }


    // NEW EDUCATIONAL ACHIEVEMENTS (Higher XP values for overtaking Spartan Warrior)

    if (!_achievement_socratic_method && tests_passed >= 15 && tests_failed == 0 && _sisyphus_xp_earned >= 400 && _sisyphus_achievements_unlocked >= 3) {
        _achievement_socratic_method = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 45 / xp_multiplier; // Socrates' 45 years of teaching
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_PURPLE "🏆 ACHIEVEMENT UNLOCKED: 🤔 Socratic Method! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   'I know that I know nothing' - learned from questioning, achieved wisdom!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Socratic Method", bonus_xp, "'I know that I know nothing' - learned from questioning, achieved wisdom!");
    }

    if (!_achievement_alexander_conquest && tests_failed == 0 && _sisyphus_xp_earned >= 600 && _total_test_time < 0.001 && tests_passed >= 12) {
        _achievement_alexander_conquest = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 50 / xp_multiplier; // Alexander conquered known world in 50 major battles
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_GOLD "🏆 ACHIEVEMENT UNLOCKED: ⚔️ Alexander's Conquest! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_BOLD_WHITE "   🌍 Conquered the known world with lightning speed and perfect strategy! 🌍" TEST_COLOR_RESET "\n");
        printf(TEST_COLOR_GRAY "   600+ XP, sub-1ms, 12+ perfect tests - legendary domination!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Alexander's Conquest", bonus_xp, "Conquered the known world with lightning speed and perfect strategy!");
    }

    if (!_achievement_euclidean_proof && tests_passed >= 9 && tests_failed == 0 && _sisyphus_max_combo >= 12 && _sisyphus_fastest_test < 0.00002) {
        _achievement_euclidean_proof = 1;
        _sisyphus_achievements_unlocked++;
        int bonus_xp = 42 / xp_multiplier; // Euclid's 42 propositions in Elements Book 1
        _sisyphus_xp_earned += bonus_xp;
        _sisyphus_xp_from_achievements += bonus_xp;
        printf(TEST_COLOR_BLUE "🏆 ACHIEVEMENT UNLOCKED: 📐 Euclidean Proof! " TEST_COLOR_YELLOW "+%d XP" TEST_COLOR_RESET "\n", bonus_xp);
        printf(TEST_COLOR_GRAY "   Q.E.D. - Mathematical precision with 12+ perfect proofs and lightning speed!" TEST_COLOR_RESET "\n");
        UPDATE_BEST_ACHIEVEMENT("Euclidean Proof", bonus_xp, "Q.E.D. - Mathematical precision with 12+ perfect proofs and lightning speed!");
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

// Epic Sisyphus gamified test assertion macro
#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            /* Reset combo suppression on failure */ \
            if (_sisyphus_suppressing_combos) { \
                printf(TEST_COLOR_GREEN "🔥 COMBO x%d VICTORY: %s" TEST_COLOR_RESET, _sisyphus_combo_multiplier - 1, _sisyphus_last_combo_message); \
                printf(TEST_COLOR_YELLOW " (+3 XP)" TEST_COLOR_RESET "\n"); \
                _sisyphus_suppressing_combos = 0; \
            } \
            _sisyphus_same_message_count = 0; \
            _sisyphus_last_combo_message[0] = '\0'; \
            _sisyphus_current_test_penalty = (tests_failed + 1) * 100; \
            printf(TEST_COLOR_RED "💥 TRIAL FAILED: %s" TEST_COLOR_RESET, message); \
            printf(TEST_COLOR_RED " (-%d XP)" TEST_COLOR_RESET "\n", _sisyphus_current_test_penalty); \
            printf(TEST_COLOR_GRAY "   🏛️  Line %d in %s" TEST_COLOR_RESET "\n", __LINE__, __FILE__); \
            if (_sisyphus_failed_count < 10) { \
                snprintf(_sisyphus_failed_tests[_sisyphus_failed_count], 256, "%s", __func__); \
                snprintf(_sisyphus_failed_messages[_sisyphus_failed_count], 512, "%s", message); \
                snprintf(_sisyphus_failed_locations[_sisyphus_failed_count], 256, "%s:%d", __FILE__, __LINE__); \
                _sisyphus_failed_count++; \
            } \
            _sisyphus_combo_multiplier = 1; \
            _sisyphus_perfect_streak = 0; \
            return 0; \
        } else { \
            _sisyphus_trials_conquered++; \
            /* Implement diminishing returns after combo x7 (8+ assertions) */ \
            int base_xp; \
            if (_sisyphus_combo_multiplier <= 7) { \
                base_xp = 10 + (3 * (_sisyphus_combo_multiplier - 1)); \
            } else { \
                /* After x7, diminish by 5 per level: 28->23->18->13->8->3 (min 3) */ \
                int diminish_levels = _sisyphus_combo_multiplier - 7; \
                base_xp = 28 - (diminish_levels * 5); \
                if (base_xp < 3) base_xp = 3; \
            } \
            int xp_gain = _sisyphus_is_full_test_mode ? (base_xp) : base_xp; \
            _sisyphus_xp_earned += xp_gain; \
            if (_sisyphus_combo_multiplier > 1) { \
                int combo_portion = xp_gain - (_sisyphus_is_full_test_mode ? (10 / 4) : 10); \
                int test_portion = _sisyphus_is_full_test_mode ? (10 / 4) : 10; \
                _sisyphus_xp_from_combos += combo_portion; \
                _sisyphus_xp_from_tests += test_portion; \
            } else { \
                _sisyphus_xp_from_tests += xp_gain; \
            } \
            /* Check if this is the same message as before */ \
            if (strcmp(_sisyphus_last_combo_message, message) == 0) { \
                _sisyphus_same_message_count++; \
                if (_sisyphus_same_message_count == 3) { \
                    /* Starting to suppress - show the third one then ellipsis */ \
                    printf(TEST_COLOR_GREEN "🔥 COMBO x%d VICTORY: %s" TEST_COLOR_RESET, _sisyphus_combo_multiplier, message); \
                    printf(TEST_COLOR_YELLOW " (+%d XP)" TEST_COLOR_RESET "\n", xp_gain); \
                    printf(TEST_COLOR_GRAY "...\n" TEST_COLOR_RESET); \
                    _sisyphus_suppressing_combos = 1; \
                    _sisyphus_first_combo_of_sequence = _sisyphus_combo_multiplier - 2; \
                } else if (_sisyphus_same_message_count < 3) { \
                    /* Show first 3 normally */ \
                    printf(TEST_COLOR_GREEN "🔥 COMBO x%d VICTORY: %s" TEST_COLOR_RESET, _sisyphus_combo_multiplier, message); \
                    printf(TEST_COLOR_YELLOW " (+%d XP)" TEST_COLOR_RESET "\n", xp_gain); \
                } \
                /* If > 3, we're suppressing - don't print */ \
            } else { \
                /* Different message - check if we were suppressing */ \
                if (_sisyphus_suppressing_combos && _sisyphus_same_message_count > 3) { \
                    /* Show the final one from the suppressed sequence */ \
                    printf(TEST_COLOR_GREEN "🔥 COMBO x%d VICTORY: %s" TEST_COLOR_RESET, _sisyphus_combo_multiplier - 1, _sisyphus_last_combo_message); \
                    printf(TEST_COLOR_YELLOW " (+3 XP)" TEST_COLOR_RESET "\n"); \
                } \
                /* Reset and show new message */ \
                _sisyphus_same_message_count = 1; \
                _sisyphus_suppressing_combos = 0; \
                strncpy(_sisyphus_last_combo_message, message, sizeof(_sisyphus_last_combo_message) - 1); \
                printf(TEST_COLOR_GREEN "🔥 COMBO x%d VICTORY: %s" TEST_COLOR_RESET, _sisyphus_combo_multiplier, message); \
                printf(TEST_COLOR_YELLOW " (+%d XP)" TEST_COLOR_RESET "\n", xp_gain); \
            } \
            _sisyphus_combo_multiplier++; \
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
            printf(TEST_COLOR_CYAN "\n🏛️ COLOSSEUM BOUT: %s" TEST_COLOR_RESET, #test_func); \
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
        if (elapsed_time < 0.00003) { \
            _sisyphus_consecutive_fast_tests++; \
        } else { \
            _sisyphus_consecutive_fast_tests = 0; \
        } \
        if (test_result) { \
            tests_passed++; \
            _sisyphus_perfect_streak++; \
            _sisyphus_current_perfect_streak++; \
            /* Track max combo using combo multiplier reached during test */ \
            int max_combo_this_test = _sisyphus_combo_multiplier - 1; \
            if (max_combo_this_test > _sisyphus_max_combo) _sisyphus_max_combo = max_combo_this_test; \
            int battle_xp = _sisyphus_trials_conquered * 5; \
            int final_xp = _sisyphus_is_full_test_mode ? (battle_xp / 4) : battle_xp; \
            _sisyphus_xp_earned += final_xp; \
            _sisyphus_xp_from_tests += final_xp; \
            printf(TEST_COLOR_GREEN "🎉 CHAMPION VICTORIOUS: %s" TEST_COLOR_RESET, #test_func); \
            printf(TEST_COLOR_BOLD_WHITE " (%.6fs)" TEST_COLOR_RESET, elapsed_time); \
            if (_sisyphus_perfect_streak >= 3) { \
                printf(TEST_COLOR_PURPLE " 🔥STREAK x%d!" TEST_COLOR_RESET, _sisyphus_perfect_streak); \
            } \
            printf("\n"); \
            if (_sisyphus_trials_conquered > 0 && !_sisyphus_is_full_test_mode) { \
                printf(TEST_COLOR_PURPLE "   🏛️ Won %d gladiator bouts! " TEST_COLOR_RESET, _sisyphus_trials_conquered); \
                printf(TEST_COLOR_YELLOW "+%d Arena XP" TEST_COLOR_RESET "\n", final_xp); \
            } \
            if (!_sisyphus_is_full_test_mode) { \
                _level_up_check(); \
            } \
        } else { \
            tests_failed++; \
            _sisyphus_beasts_slayed += _sisyphus_trials_conquered; \
            printf(TEST_COLOR_RED "💀 HEROIC DEFEAT: %s" TEST_COLOR_RESET, #test_func); \
            printf(TEST_COLOR_BOLD_WHITE " (%.6fs)" TEST_COLOR_RESET "\n", elapsed_time); \
            if (_sisyphus_trials_conquered > 0) { \
                printf(TEST_COLOR_ORANGE "   🔥 But you got %d combo%s before failing!" TEST_COLOR_RESET "\n", _sisyphus_trials_conquered, _sisyphus_trials_conquered == 1 ? "" : "s"); \
            } \
            _sisyphus_perfect_streak = 0; \
            _sisyphus_current_perfect_streak = 0; \
        } \
        _sisyphus_trials_conquered = 0; \
        _sisyphus_combo_multiplier = 1; \
        /* End any suppressed combo sequence when test ends */ \
        if (_sisyphus_suppressing_combos && _sisyphus_same_message_count > 3) { \
            printf(TEST_COLOR_GREEN "🔥 COMBO x%d VICTORY: %s" TEST_COLOR_RESET, _sisyphus_combo_multiplier - 1, _sisyphus_last_combo_message); \
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
        printf(TEST_COLOR_PURPLE "⚔️  ENTERING THE BATTLE ARENA: %s" TEST_COLOR_RESET "\n", suite_name); \
        printf(TEST_COLOR_CYAN "════════════════════════════════════════════════════════" TEST_COLOR_RESET "\n"); \
        printf(TEST_COLOR_YELLOW "🔥 Prepare for combat! Each test is a battle for glory!" TEST_COLOR_RESET "\n"); \
        printf(TEST_COLOR_GRAY "💀 Slay beasts • Chain victories • Unlock battle achievements!" TEST_COLOR_RESET "\n"); \
        char total_xp_str[32], next_level_xp_str[32]; \
        format_number_with_commas(total_xp_str, sizeof(total_xp_str), _sisyphus_total_project_xp); \
        format_number_with_commas(next_level_xp_str, sizeof(next_level_xp_str), _get_xp_for_level(_sisyphus_project_level + 1)); \
        printf(TEST_COLOR_CYAN "🏆 Gladiator Level: %d | Total Battle XP: %s | Next Level: %s XP" TEST_COLOR_RESET "\n", _sisyphus_project_level, total_xp_str, next_level_xp_str); \
    } else { \
        printf(TEST_COLOR_GRAY "🏛️  Colosseum Suite: %s" TEST_COLOR_RESET " ", suite_name); \
    }

#define TEST_SUITE_END() \
    double _suite_end_time = get_time_precise(); \
    double _total_suite_time = _suite_end_time - _test_suite_start_time; \
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
    printf(TEST_COLOR_PURPLE "🏛️ FINAL COLOSSEUM STATS - ARENA MASTERY REPORT" TEST_COLOR_RESET "\n"); \
    printf(TEST_COLOR_CYAN "════════════════════════════════════════════════════════" TEST_COLOR_RESET "\n"); \
    _display_progress_bar(tests_passed, total_tests, "Victory Rate", TEST_COLOR_GREEN); \
    printf(TEST_COLOR_CYAN "🧪 Test Functions Executed: %d" TEST_COLOR_RESET " | ", total_tests); \
    printf(TEST_COLOR_GREEN "🏆 Won: %d" TEST_COLOR_RESET " | ", tests_passed); \
    printf(TEST_COLOR_RED "💀 Lost: %d" TEST_COLOR_RESET "\n", tests_failed); \
    int current_level_xp = _get_xp_in_current_level(_sisyphus_xp_earned, _sisyphus_current_level); \
    int xp_needed_for_next = _get_xp_for_level(_sisyphus_current_level); \
    _display_progress_bar_with_level(current_level_xp, xp_needed_for_next, "Suite XP", TEST_COLOR_YELLOW, _sisyphus_current_level, 0); \
    int project_level_xp = _get_xp_in_current_level(_sisyphus_total_project_xp, _sisyphus_project_level); \
    int project_xp_needed = _get_xp_for_level(_sisyphus_project_level); \
    _display_progress_bar_with_level(project_level_xp, project_xp_needed, "Project XP", TEST_COLOR_PURPLE, _sisyphus_project_level, 1); \
    printf(TEST_COLOR_BLUE "⏱️  Fastest Trial: %.6fs" TEST_COLOR_RESET " | ", _sisyphus_fastest_test < 999.0 ? _sisyphus_fastest_test : 0.0); \
    printf(TEST_COLOR_GREEN "🏆 Achievements: %d" TEST_COLOR_RESET, _sisyphus_achievements_unlocked); \
    if (_sisyphus_best_achievement_xp > 0) { \
        printf(TEST_COLOR_BOLD_WHITE " | Best Achievement: %s (%d XP)" TEST_COLOR_RESET, _sisyphus_best_achievement, _sisyphus_best_achievement_xp); \
    } \
    printf("\n"); \
    printf(TEST_COLOR_GRAY "🕒 Arena Time: %.6fs | 🔧 Execution: %.6fs | 📊 Framework: %.6fs" TEST_COLOR_RESET "\n", \
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
        printf(TEST_COLOR_ORANGE "\n⚔️  HONORABLE DEFEAT! Warriors learn from every battle!" TEST_COLOR_RESET "\n"); \
        char level_xp_str[32]; \
        format_number_with_commas(level_xp_str, sizeof(level_xp_str), _sisyphus_xp_earned); \
        printf(TEST_COLOR_YELLOW "🎮 Level %d achieved with %s XP - Progress never stops!" TEST_COLOR_RESET "\n", _sisyphus_current_level, level_xp_str); \
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
    printf(TEST_COLOR_GRAY "⏱️  %.6fs | 🏆 %d achievements | ✨ %.6fs fastest" TEST_COLOR_RESET "\n", \
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
        printf(TEST_COLOR_PURPLE "🎮 Perfect run! +%s XP integrated to Project Level %d!" TEST_COLOR_RESET "\n", perfect_xp_str, _sisyphus_project_level); \
        return 0; \
    } else { \
        char gained_xp_str[32]; \
        format_number_with_commas(gained_xp_str, sizeof(gained_xp_str), _sisyphus_xp_earned); \
        printf(TEST_COLOR_ORANGE "⚔️  +%s XP gained! Project Level %d progress saved!" TEST_COLOR_RESET "\n", gained_xp_str, _sisyphus_project_level); \
        return 1; \
    }

#endif
