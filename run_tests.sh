#!/bin/bash
# File: run_tests.sh - Enhanced Test Runner with Sisyphus Progress Tracking
# This testing framework is designed for Dopamine-driven development
# It should Gamify, and help coders with ADHD feel a sense of accomplishment.
# It should also provide a thematic experience within the world of Greek mythology.
# The Sisyphus legend serves as a metaphor for perseverance and the pursuit of perfection.

# Set environment variable to indicate this is a full test run (1/N XP where N = successfully run files, no achievements)
export SISYPHUS_FULL_TEST_MODE=1

RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
PURPLE='\033[0;35m'
BOLD_WHITE='\033[1;37m'
CYAN='\033[0;36m'
ORANGE='\033[0;93m'
NC='\033[0m'

# Sisyphus archive system for tracking progress
SISYPHUS_DIR=".sisyphus"
SISYPHUS_ARCHIVE_DIR="$SISYPHUS_DIR/archive"  # Stats files archive
SISYPHUS_FILE=".sisyphus"  # Legacy file for migration
SISYPHUS_LEGEND="sisyphus.legend"  # Permanent lifetime stats
SISYPHUS_DAILY_MILESTONES="$SISYPHUS_DIR/daily_milestones"  # Daily milestone tracking

# Color codes for faded italic explanations
FADED='\033[2;3m'  # Faded italic text
GRAY='\033[2m'     # Dim/faded text

# Function to format numbers with commas (e.g., 107020 -> 107,020)
format_number_with_commas() {
    local number="$1"
    echo "$number" | sed ':a;s/\B[0-9]\{3\}\>/,&/;ta'
}

# Function to map make targets to correct test file paths
# Used for generating clickable hyperlinks in terminal output

map_target_to_file_path() {
    local target="$1"
    case "$target" in
        "run-test-create-string-from-file")
            echo "true_tests/strings/test_create_string_from_file.c"
            ;;
        "run-test-string-builder")
            echo "true_tests/strings/test_string_builder.c"
            ;;
        "run-test-string-builder-edge")
            echo "true_tests/strings/test_string_builder_edge.c"
            ;;
        "run-test-string-advanced")
            echo "true_tests/strings/test_string_advanced.c"
            ;;
        "run-test-string-padding")
            echo "true_tests/strings/test_string_padding.c"
            ;;
        "run-test-string-pythonic")
            echo "true_tests/strings/test_string_pythonic.c"
            ;;
        "run-test-dynamic-array-basic")
            echo "true_tests/dynamicarrays/test_dynamic_array_basic.c"
            ;;
        "run-test-dynamic-array-edge")
            echo "true_tests/dynamicarrays/test_dynamic_array_edge.c"
            ;;
        "run-test-dynamic-array-resize")
            echo "true_tests/dynamicarrays/test_dynamic_array_resize.c"
            ;;
        "run-test-dynamic-array-performance")
            echo "true_tests/dynamicarrays/test_dynamic_array_performance.c"
            ;;
        "run-test-dynamic-array-advanced")
            echo "true_tests/dynamicarrays/test_dynamic_array_advanced.c"
            ;;
        "run-test-dynamic-array-errors")
            echo "true_tests/dynamicarrays/test_dynamic_array_errors.c"
            ;;
        *)
            # Fallback for unknown targets
            echo "true_tests/${target#*test-}.c"
            ;;
    esac
}
# Function to copy text to clipboard using OSC 52 (works in many terminals)
copy_to_clipboard() {
    local text="$1"
    # OSC 52 sequence to copy to clipboard
    printf '\e]52;c;%s\e\\' "$(printf '%s' "$text" | base64 | tr -d '\n')"
}

# Greek Mythology-themed Sisyphus messages for dopamine-driven development
LEGENDARY_MESSAGES=(
    "✨ ZEUS HIMSELF APPLAUDS! Your boulder has reached Olympus!"
    "👑 DIVINE ASCENSION! You've earned the respect of the gods!"
    "🏛️ LEGENDARY TRIUMPH! Sisyphus weeps with joy at your mastery!"
    "🌟 IMMORTAL ACHIEVEMENT! Your name shall echo through eternity!"
    "🔥 PROMETHEUS PROUD! You've stolen the fire of perfect code!"
)

EXCELLENT_MESSAGES=(
    "💎 HERCULEAN STRENGTH! Your twelve labors of debugging are complete!"
    "🚀 ICARUS ASCENDANT! Flying high without melting your wings!"
    "⭐ ATHENA'S WISDOM! Strategic debugging guided by divine intellect!"
    "💪 ATLAS STRONG! Carrying the weight of complex code with ease!"
    "🎯 APOLLO'S AIM! Each bug elimination precise as golden arrows!"
)

GOOD_MESSAGES=(
    "✨ SISYPHEAN PROGRESS! Each push moves the boulder higher!"
    "🎨 HEPHAESTUS CRAFT! Forging code with divine artisanship!"
    "🛠️ DAEDALUS BUILDER! Constructing labyrinths of elegant logic!"
    "🏗️ FOUNDATION OF TROY! Your code stands strong against all sieges!"
    "📈 HERMES SPEED! Swift progress carries messages of success!"
)

ENCOURAGEMENT_MESSAGES=(
    "🌱 PERSEPHONE'S RETURN! From darkness comes renewed growth!"
    "🎪 ODYSSEY CONTINUES! Every hero faces trials before triumph!"
    "🦾 TITAN STRENGTH! Growing mightier with each challenge!"
    "🧩 SPHINX'S RIDDLE! You love solving the impossible, don't you?"
    "✨ SPARTAN SPIRIT! This code shall not pass without a fight!"
    "🏛️ EPICTETUS REMINDS! 'It's not what happens, but how you react!'"
    "📚 SENECA'S WISDOM! 'Every new beginning comes from some other beginning's end!'"
    "✨ MARCUS AURELIUS! 'The impediment to action advances action. What stands in the way becomes the way!'"
    "🎯 ARISTOTLE'S VIRTUE! Excellence is not an act, but a habit - keep coding!"
    "🌟 SOCRATIC METHOD! Question everything, improve iteratively!"
)

STABLE_MESSAGES=(
    "🧘 STOIC PHILOSOPHER! Wisdom lies in consistent excellence!"
    "🛡️ TROJAN WALLS! Your defenses hold strong against chaos!"
    "⚖️ THEMIS BALANCE! Divine justice maintains perfect equilibrium!"
    "🎯 ARTEMIS FOCUS! Patient hunter waiting for the perfect moment!"
    "🔒 PANDORA'S BOX! Your code remains sealed from corruption!"
    "🧘 EPICTETUS TEACHES! Focus only on what you can control - your code!"
    "⚖️ ARISTOTLE'S MEAN! Perfect balance between haste and contemplation!"
    "🏛️ SOCRATIC WISDOM! 'I know that I know nothing' - but my tests pass!"
    "📜 MARCUS AURELIUS! 'Confine yourself to the present' - this moment, this code!"
    "🌊 ZENO'S PARADOX! Each step brings you infinitely closer to perfection!"
    "🎭 PLATO'S CAVE! Your stable code reflects eternal Forms of Truth!"
)

# Story-driven symbols that reflect the Sisyphus/Daedalus narrative
NARRATIVE_SYMBOLS=(
    "🪨 THE BOULDER'S WEIGHT" "🗿 DAEDALUS'S CHISEL" "🏛️ MINOS'S DECREE" "🕊️ ICARUS'S FEATHER" "🐂 THE MINOTAUR'S ROAR"
    "🌀 THE LABYRINTH'S SPIRAL" "⛓️ SISYPHUS'S CHAINS" "📜 THE ARCHITECT'S BLUEPRINT" "🔥 THE FORGE'S FLAME" "👑 THE KING'S SHADOW"
)

echo -e "${PURPLE}🚀 Daedalus Test Suite Runner${NC}"
echo -e "${CYAN}Enhanced with Sisyphus Progress Archive & Historical Analysis${NC}"
echo -e "${GRAY}Run './run_tests.sh help' for archive commands${NC}"
echo "=================================================="

# Function to read lifetime legend stats
read_legend_stats() {
    if [ -f "$SISYPHUS_LEGEND" ]; then
        local total_runs=0
        local total_quests=0
        local hydra_beasts_slain=0
        local blueprint_flaws_fixed=0
        local labyrinth_corridors_explored=0
        local divine_streaks_achieved=0
        local longest_streak=0
        local perfect_ascensions=0
        local first_quest_date=""
        local total_project_xp=0
        local project_level=1

        while IFS='=' read -r key value; do
            case "$key" in
                "TOTAL_RUNS") total_runs="$value";;
                "TOTAL_QUESTS") total_quests="$value";;
                "HYDRA_BEASTS_SLAIN") hydra_beasts_slain="$value";;
                "BLUEPRINT_FLAWS_FIXED") blueprint_flaws_fixed="$value";;
                "LABYRINTH_CORRIDORS_EXPLORED") labyrinth_corridors_explored="$value";;
                "DIVINE_STREAKS_ACHIEVED") divine_streaks_achieved="$value";;
                "LONGEST_STREAK") longest_streak="$value";;
                "PERFECT_ASCENSIONS") perfect_ascensions="$value";;
                "FIRST_QUEST_DATE") first_quest_date="$value";;
                "TOTAL_PROJECT_XP") total_project_xp="$value";;
                "PROJECT_LEVEL") project_level="$value";;
            esac
        done < "$SISYPHUS_LEGEND"

        # Set first quest date if empty
        if [ -z "$first_quest_date" ]; then
            first_quest_date="$(date '+%Y-%m-%d')"
        fi
        echo "$total_runs|$total_quests|$hydra_beasts_slain|$blueprint_flaws_fixed|$labyrinth_corridors_explored|$divine_streaks_achieved|$longest_streak|$perfect_ascensions|$first_quest_date|$total_project_xp|$project_level"
    else
        echo "0|0|0|0|0|0|0|0|$(date '+%Y-%m-%d')|0|1"
    fi
}

# Function to update lifetime legend stats
update_legend_stats() {
    local curr_runtime_errors="$1"
    local curr_compile_errors="$2"
    local curr_test_failures="$3"
    local curr_streak="$4"
    local is_perfect_run="$5"
    local curr_challenge="$6"
    local session_xp="${7:-0}"

    # Read current legend stats
    local legend_line="$(read_legend_stats)"
    IFS='|' read -r total_runs total_quests hydra_beasts blueprint_flaws oracle_mysteries divine_streaks longest_streak perfect_ascensions first_quest total_project_xp project_level <<< "$legend_line"
    # CRITICAL FIX: Actually add session XP to total project XP!
    total_project_xp=$((total_project_xp + session_xp))
    # Get previous run stats for intelligent quest tracking
    local prev_stats_line="$(read_previous_stats)"
    IFS='|' read -r prev_errors prev_passes prev_failures prev_compile_errors prev_runtime_errors prev_efficiency_ratio prev_pure_test_time prev_improvement_streak prev_current_challenge prev_timestamp <<< "$prev_stats_line"

    # Set first quest date if empty
    if [ -z "$first_quest" ]; then
        first_quest="$(date '+%Y-%m-%d')"
    fi

    # Always increment run count
    total_runs=$((total_runs + 1))

    # Quest completion logic - only count completed quests
    local quest_completed=0

    # Check if quest was completed this run
    if [ "$curr_challenge" = "🏛️ Mount Olympus Achieved" ] && [ "$prev_current_challenge" != "🏛️ Mount Olympus Achieved" ]; then
        # Achieved perfection - quest complete
        quest_completed=0
    elif [ "$prev_current_challenge" != "" ] && [ "$prev_current_challenge" != "$curr_challenge" ] && [ "$prev_current_challenge" != "🏛️ Mount Olympus Achieved" ]; then
        # Challenge type changed (e.g., from Hydra to Workshop) - previous quest complete
        quest_completed=1
    elif [ "$prev_current_challenge" != "" ] && [ "$prev_current_challenge" != "🏛️ Mount Olympus Achieved" ]; then
        # Check if same quest type but count reached zero
        prev_challenge_type=$(echo "$prev_current_challenge" | sed 's/([^)]*)//g' | sed 's/[0-9]//g')
        curr_challenge_type=$(echo "$curr_challenge" | sed 's/([^)]*)//g' | sed 's/[0-9]//g')

        if [ "$prev_challenge_type" = "$curr_challenge_type" ]; then
            # Same challenge type - extract numbers to see if we completed
            prev_count=$(echo "$prev_current_challenge" | grep -o '[0-9]\+' | head -1)
            curr_count=$(echo "$curr_challenge" | grep -o '[0-9]\+' | head -1)

            if [ -n "$prev_count" ] && [ "$prev_count" -gt 0 ] && ([ -z "$curr_count" ] || [ "${curr_count:-0}" -eq 0 ]); then
                quest_completed=1
            fi
        fi
    fi

    # Increment quest count only if quest was completed
    if [ "$quest_completed" -eq 1 ]; then
        total_quests=$((total_quests + 1))
    fi

    # Intelligent monster slaying - only count actual progress (reductions)
    local runtime_beasts_slayed=0
    local compile_flaws_fixed=0
    local corridors_explored=0

    # Only count if we actually made progress (reduced errors)
    if [ -n "$prev_runtime_errors" ] && [ "$prev_runtime_errors" -gt "$curr_runtime_errors" ]; then
        runtime_beasts_slayed=$((prev_runtime_errors - curr_runtime_errors))
        hydra_beasts=$((hydra_beasts + runtime_beasts_slayed))
    fi

    if [ -n "$prev_compile_errors" ] && [ "$prev_compile_errors" -gt "$curr_compile_errors" ]; then
        compile_flaws_fixed=$((prev_compile_errors - curr_compile_errors))
        blueprint_flaws=$((blueprint_flaws + compile_flaws_fixed))
    fi

    if [ -n "$prev_failures" ] && [ "$prev_failures" -gt "$curr_test_failures" ]; then
        corridors_explored=$((prev_failures - curr_test_failures))
        oracle_mysteries=$((oracle_mysteries + corridors_explored))
    fi

    # Update streak achievements
    if [ "${curr_streak:-0}" -ge 3 ] && [ "${curr_streak:-0}" -gt "${longest_streak:-0}" ]; then
        divine_streaks_achieved=$((divine_streaks_achieved + 1))
        longest_streak="$curr_streak"
    fi

    # Update perfect runs
    if [ "$is_perfect_run" = "true" ]; then
        perfect_ascensions=$((perfect_ascensions + 1))
    fi

    # Calculate correct project level from total XP
    local correct_project_level=1
    local cumulative_xp=0
    while true; do
        local xp_needed=$(( 1000 + (correct_project_level * correct_project_level * correct_project_level * 200) + (correct_project_level * correct_project_level * 600) + (correct_project_level * 300) ))
        if [ $((cumulative_xp + xp_needed)) -gt $total_project_xp ]; then
            break
        fi
        cumulative_xp=$((cumulative_xp + xp_needed))
        correct_project_level=$((correct_project_level + 1))
    done

    # Write updated legend stats
    cat > "$SISYPHUS_LEGEND" << EOF
# Sisyphus Eternal Legend - Lifetime Achievements
# Hero's journey began: $first_quest
TOTAL_RUNS=$total_runs
TOTAL_QUESTS=$total_quests
HYDRA_BEASTS_SLAIN=$hydra_beasts
BLUEPRINT_FLAWS_FIXED=$blueprint_flaws
LABYRINTH_CORRIDORS_EXPLORED=$oracle_mysteries
DIVINE_STREAKS_ACHIEVED=$divine_streaks
LONGEST_STREAK=$longest_streak
PERFECT_ASCENSIONS=$perfect_ascensions
FIRST_QUEST_DATE=$first_quest
TOTAL_PROJECT_XP=$total_project_xp
PROJECT_LEVEL=$correct_project_level
EOF
}

# Function to migrate legacy .sisyphus file to new archive system
migrate_legacy_sisyphus() {
    if [ -f "$SISYPHUS_FILE" ] && [ ! -d "$SISYPHUS_DIR" ]; then
        echo -e "${YELLOW}🏛️ Migrating to Sisyphus Archive System...${NC}"
        mkdir -p "$SISYPHUS_DIR"

        # Get timestamp from legacy file or use current time
        local legacy_timestamp=$(grep "TIMESTAMP=" "$SISYPHUS_FILE" | cut -d'=' -f2 | tr -d ' ')
        if [ -z "$legacy_timestamp" ] || [ "$legacy_timestamp" = "never" ]; then
            legacy_timestamp=$(date '+%Y-%m-%d %H:%M:%S')
        fi

        # Convert timestamp to reverse chronological filename
        local filename=$(echo "$legacy_timestamp" | sed 's/[: -]//g' | sed 's/\(..\)\(..\)\(..\)\(..\)\(..\)\(..\)/\6\5\4\3\2\1/')

        # Copy legacy file to archive with new name
        cp "$SISYPHUS_FILE" "$SISYPHUS_DIR/${filename}.sp-stats"

        # Keep legacy file as backup
        mv "$SISYPHUS_FILE" "${SISYPHUS_FILE}.backup"
        echo -e "${GREEN}✅ Migration complete! Legacy file backed up.${NC}"
    fi
}

# Function to get latest stats file from archive
get_latest_stats_file() {
    if [ ! -d "$SISYPHUS_ARCHIVE_DIR" ]; then
        echo ""
        return
    fi

    # Find the latest .sp-stats file (chronological order - newest sorts last)
    local latest_file=$(ls -1 "$SISYPHUS_ARCHIVE_DIR"/*.sp-stats 2>/dev/null | tail -1)
    echo "$latest_file"
}

# Function to generate timestamped filename (chronological for natural sorting)
generate_stats_filename() {
    local timestamp=$(date '+%Y%m%d%H%M%S')
    # Ensure archive directory exists
    mkdir -p "$SISYPHUS_ARCHIVE_DIR"
    # Use normal timestamp format so newer files sort last alphabetically
    echo "${SISYPHUS_ARCHIVE_DIR}/${timestamp}.sp-stats"
}

# Function to read previous stats from archive system
read_previous_stats() {
    # First migrate if needed
    migrate_legacy_sisyphus

    local latest_file=$(get_latest_stats_file)

    if [ -n "$latest_file" ] && [ -f "$latest_file" ]; then
        local prev_errors=0
        local prev_passes=0
        local prev_failures=0
        local prev_compile_errors=0
        local prev_runtime_errors=0
        local prev_efficiency_ratio=0
        local prev_pure_test_time=0
        local prev_improvement_streak=0
        local prev_current_challenge=""
        local prev_timestamp=""

        while IFS='=' read -r key value; do
            case "$key" in
                "ERRORS") prev_errors="$value";;
                "PASSES") prev_passes="$value";;
                "FAILURES") prev_failures="$value";;
                "COMPILE_ERRORS") prev_compile_errors="$value";;
                "RUNTIME_ERRORS") prev_runtime_errors="$value";;
                "EFFICIENCY_RATIO") prev_efficiency_ratio="$value";;
                "PURE_TEST_TIME") prev_pure_test_time="$value";;
                "IMPROVEMENT_STREAK") prev_improvement_streak="$value";;
                "CURRENT_CHALLENGE") prev_current_challenge="$value";;
                "TIMESTAMP") prev_timestamp="$value";;
            esac
        done < "$latest_file"

        echo "$prev_errors|$prev_passes|$prev_failures|$prev_compile_errors|$prev_runtime_errors|$prev_efficiency_ratio|$prev_pure_test_time|$prev_improvement_streak|$prev_current_challenge|$prev_timestamp"
    else
        echo "0|0|0|0|0|0|0.000000|0||never"
    fi
}

# Function to get random message from array
get_random_message() {
    local -n arr=$1
    echo "${arr[$RANDOM % ${#arr[@]}]}"
}

# Function to get random narrative symbol
get_narrative_symbol() {
    echo "${NARRATIVE_SYMBOLS[$RANDOM % ${#NARRATIVE_SYMBOLS[@]}]}"
}

# Function to generate current challenge
# Generate current challenge based on remaining issues
generate_current_challenge() {
    local failed_files=("$@")

    if [ ${#failed_files[@]} -eq 0 ]; then
        echo "none"
        return
    fi

    # Get random failed test
    local random_index=$((RANDOM % ${#failed_files[@]}))
    local selected_failure="${failed_files[$random_index]}"

    # Extract the make target from the failure string
    local make_target=$(echo "$selected_failure" | cut -d':' -f2)
    echo "$make_target"
}

# New technical achievement detection
check_technical_achievements() {
    local curr_errors="$1"
    local curr_passes="$2"
    local curr_efficiency="$3"
    local test_time="$4"

    # Check for new achievement unlocks
    if [ "$curr_errors" -eq 0 ] && [ "$curr_passes" -gt 50 ]; then
        echo -e "${PURPLE}🏆 ACHIEVEMENT: Marathon Runner - 50+ tests passing with zero errors${NC}"
    fi

    if (( $(echo "$test_time < 0.001" | bc -l) )) && [ "$curr_passes" -gt 10 ]; then
        echo -e "${CYAN}🏆 ACHIEVEMENT: Lightning Fast - 10+ tests in under 1ms${NC}"
    fi

    if [ "$curr_efficiency" -gt 1000 ]; then
        echo -e "${YELLOW}🏆 ACHIEVEMENT: Optimization Master - 1000x+ execution efficiency${NC}"
    fi

    # Streak-based achievements
    local stats_line="$(read_previous_stats)"
    IFS='|' read -r prev_errors prev_passes prev_failures prev_compile_errors prev_runtime_errors prev_efficiency_ratio prev_pure_test_time prev_improvement_streak prev_current_challenge prev_timestamp <<< "$stats_line"

    if [ "$prev_improvement_streak" -ge 7 ]; then
        echo -e "${GREEN}🏆 ACHIEVEMENT: Consistency Expert - 7+ consecutive improvement sessions${NC}"
    fi
}

# Function to analyze historical trends from archive
analyze_historical_trends() {
    if [ ! -d "$SISYPHUS_ARCHIVE_DIR" ]; then
        return
    fi

    local files=($(ls -1 "$SISYPHUS_ARCHIVE_DIR"/*.sp-stats 2>/dev/null))
    local file_count=${#files[@]}

    if [ "$file_count" -lt 2 ]; then
        return
    fi

    # Analyze last 5 runs for trends
    local recent_files=($(ls -1 "$SISYPHUS_ARCHIVE_DIR"/*.sp-stats 2>/dev/null | head -5))
    local improvements=0
    local regressions=0
    local max_streak=0
    local perfect_runs=0

    for file in "${recent_files[@]}"; do
        local errors=$(grep "ERRORS=" "$file" | cut -d'=' -f2 | tr -d '\n\r' | tr -d ' ')
        local streak=$(grep "IMPROVEMENT_STREAK=" "$file" | cut -d'=' -f2 | tr -d '\n\r' | tr -d ' ')

        if [ -n "$errors" ] && [ "${errors:-1}" -eq 0 ]; then
            perfect_runs=$((perfect_runs + 1))
        fi

        if [ -n "$streak" ] && [ "$streak" != "" ] && [[ "$streak" =~ ^[0-9]+$ ]] && [ "${streak:-0}" -gt "${max_streak:-0}" ]; then
            max_streak=$streak
        fi
    done

    # Return analysis results
    echo "$file_count|$perfect_runs|$max_streak"
}

# Function to get today's date string
get_today_date() {
    date '+%Y-%m-%d'
}

# Function to check if milestone was already shown today
was_milestone_shown_today() {
    local milestone_key="$1"
    local today=$(get_today_date)

    if [ ! -f "$SISYPHUS_DAILY_MILESTONES" ]; then
        return 1  # Not shown (file doesn't exist)
    fi

    # Check if milestone key exists for today
    grep -q "^${today}:${milestone_key}$" "$SISYPHUS_DAILY_MILESTONES" 2>/dev/null
    return $?
}

# Function to mark milestone as shown today
mark_milestone_shown_today() {
    local milestone_key="$1"
    local today=$(get_today_date)

    # Ensure .sisyphus directory exists
    mkdir -p "$SISYPHUS_DIR"

    # Add milestone to today's shown list
    echo "${today}:${milestone_key}" >> "$SISYPHUS_DAILY_MILESTONES"

    # Clean up old entries (keep only last 7 days)
    if [ -f "$SISYPHUS_DAILY_MILESTONES" ]; then
        local cutoff_date=$(date -d "7 days ago" '+%Y-%m-%d' 2>/dev/null || date -v-7d '+%Y-%m-%d' 2>/dev/null || echo "1970-01-01")
        local temp_file=$(mktemp)
        awk -F: -v cutoff="$cutoff_date" '$1 >= cutoff' "$SISYPHUS_DAILY_MILESTONES" > "$temp_file"
        mv "$temp_file" "$SISYPHUS_DAILY_MILESTONES"
    fi
}

# Function to get perfect runs count for today
get_today_perfect_runs() {
    local today=$(get_today_date)
    local today_files=()

    if [ ! -d "$SISYPHUS_ARCHIVE_DIR" ]; then
        echo "0"
        return
    fi

    # Get all files from today (format: YYYYMMDDHHMMSS.sp-stats)
    local today_pattern="${today//-/}*.sp-stats"
    local files=($(ls -1 "$SISYPHUS_ARCHIVE_DIR"/$today_pattern 2>/dev/null))

    local perfect_count=0
    local consecutive_perfect=0
    local max_consecutive=0

    # Sort files by timestamp to check consecutive perfect runs
    IFS=$'\n' sorted_files=($(sort <<< "${files[*]}"))

    for file in "${sorted_files[@]}"; do
        local errors=$(grep "ERRORS=" "$file" | cut -d'=' -f2 | tr -d '\n\r' | tr -d ' ')

        if [ -n "$errors" ] && [ ${errors:-1} -eq 0 ]; then
            perfect_count=$((perfect_count + 1))
            consecutive_perfect=$((consecutive_perfect + 1))
            if [ $consecutive_perfect -gt $max_consecutive ]; then
                max_consecutive=$consecutive_perfect
            fi
        else
            consecutive_perfect=0
        fi
    done

    echo "$perfect_count|$max_consecutive"
}

# Function to check for milestone achievements
check_milestones() {
    local analysis="$1"
    IFS='|' read -r total_runs perfect_runs max_streak <<< "$analysis"

    # Get today's perfect runs data
    local today_data=$(get_today_perfect_runs)
    IFS='|' read -r today_perfect_runs today_consecutive_perfect <<< "$today_data"

    # Ensure we have valid numbers
    today_perfect_runs=${today_perfect_runs:-0}
    today_consecutive_perfect=${today_consecutive_perfect:-0}
    total_runs=${total_runs:-0}
    perfect_runs=${perfect_runs:-0}
    max_streak=${max_streak:-0}

    local eligible_milestones=()

    # Time-based milestones (daily reset)
    current_hour=$(date +%H)
    if [ "$current_hour" -lt 6 ] && [ "$today_perfect_runs" -gt 0 ]; then
        if ! was_milestone_shown_today "night_owl"; then
            eligible_milestones+=("night_owl:🌙 NIGHT OWL! Coding excellence in the witching hours!")
        fi
    elif [ "$current_hour" -ge 5 ] && [ "$current_hour" -lt 7 ]; then
        if ! was_milestone_shown_today "dawn_warrior"; then
            eligible_milestones+=("dawn_warrior:🌅 DAWN WARRIOR! First light brings first victory!")
        fi
    fi

    # Day of week achievements (daily reset)
    day_of_week=$(date +%u)
    if [ "$today_perfect_runs" -gt 0 ]; then
        case "$day_of_week" in
            1) # Monday
                if ! was_milestone_shown_today "monday_motivation"; then
                    eligible_milestones+=("monday_motivation:💼 MONDAY MOTIVATION! Starting the week with excellence!")
                fi
                ;;
            2) # Tuesday
                if ! was_milestone_shown_today "tuesday_ares"; then
                    eligible_milestones+=("tuesday_ares:🗡️ ARES VIGOR! Tuesday strength powers through challenges!")
                fi
                ;;
            3) # Wednesday
                if ! was_milestone_shown_today "wednesday_athena"; then
                    eligible_milestones+=("wednesday_athena:🦉 ATHENA'S WISDOM! Midweek strategy conquers complexity!")
                fi
                ;;
            4) # Thursday
                if ! was_milestone_shown_today "thursday_hermes"; then
                    eligible_milestones+=("thursday_hermes:🏃 HERMES MESSENGER! Thursday brings news of weekend victory!")
                fi
                ;;
            5) # Friday
                if ! was_milestone_shown_today "friday_finisher"; then
                    eligible_milestones+=("friday_finisher:🎉 FRIDAY FINISHER! Ending the week in style!")
                fi
                ;;
            6) # Saturday
                if ! was_milestone_shown_today "saturday_apollo"; then
                    eligible_milestones+=("saturday_apollo:☀️ APOLLO'S LIGHT! Saturday sun illuminates perfect code!")
                fi
                ;;
            7) # Sunday
                if ! was_milestone_shown_today "sunday_hestia"; then
                    eligible_milestones+=("sunday_hestia:🏛️ HESTIA'S HEARTH! Sunday sanctuary of peaceful coding!")
                fi
                ;;
        esac
    fi

    # Today's consecutive perfect runs milestone (daily reset)
    if [ "$today_consecutive_perfect" -ge 3 ]; then
        if ! was_milestone_shown_today "trinity_perfection_today"; then
            eligible_milestones+=("trinity_perfection_today:💎 TRINITY PERFECTION! 3+ flawless victories in a row today!")
        fi
    fi



    # Phoenix Rising milestone (daily reset) - check if we had failures today but then achieved perfect run
    local today_failed_runs=0
    if [ -d "$SISYPHUS_ARCHIVE_DIR" ]; then
        local today_pattern="${today//-/}*.sp-stats"
        local files=($(ls -1 "$SISYPHUS_ARCHIVE_DIR"/$today_pattern 2>/dev/null))

        for file in "${files[@]}"; do
            local errors=$(grep "ERRORS=" "$file" | cut -d'=' -f2 | tr -d '\n\r' | tr -d ' ')
            if [ -n "$errors" ] && [ ${errors:-0} -gt 0 ]; then
                today_failed_runs=$((today_failed_runs + 1))
            fi
        done
    fi

    if [ "${today_failed_runs:-0}" -gt 0 ] && [ "${today_perfect_runs:-0}" -gt 0 ]; then
        if ! was_milestone_shown_today "phoenix_rising"; then
            eligible_milestones+=("phoenix_rising:🔥 PHOENIX RISING! From failure's ashes to perfect victory!")
        fi
    fi

    # Lifetime milestones (no daily reset needed)
    if [ "$total_runs" -eq 10 ]; then
        if ! was_milestone_shown_today "temple_built"; then
            eligible_milestones+=("temple_built:🏛️ TEMPLE BUILT! 10 coding sessions completed!")
        fi
    elif [ "$total_runs" -eq 50 ]; then
        if ! was_milestone_shown_today "fortress_strong"; then
            eligible_milestones+=("fortress_strong:🏰 FORTRESS STRONG! 50 legendary battles fought!")
        fi
    elif [ "$total_runs" -eq 100 ]; then
        if ! was_milestone_shown_today "immortal_status"; then
            eligible_milestones+=("immortal_status:👑 IMMORTAL STATUS! 100 divine trials conquered!")
        fi
    fi

    if [ "$max_streak" -ge 5 ]; then
        if ! was_milestone_shown_today "divine_streak"; then
            eligible_milestones+=("divine_streak:🔥 DIVINE STREAK! ${max_streak} consecutive triumphs!")
        fi
    fi

    if [ "$max_streak" -eq 7 ]; then
        if ! was_milestone_shown_today "seven_sages"; then
            eligible_milestones+=("seven_sages:🏛️ SEVEN SAGES! Your wisdom rivals the ancient philosophers!")
        fi
    elif [ "$max_streak" -eq 4 ]; then
        if ! was_milestone_shown_today "cardinal_virtues"; then
            eligible_milestones+=("cardinal_virtues:📚 CARDINAL VIRTUES! Wisdom, Justice, Fortitude, Temperance achieved!")
        fi
    fi

    # Select one random milestone if any are eligible
    if [ ${#eligible_milestones[@]} -gt 0 ]; then
        # Use current time and run count for pseudo-randomness
        local random_seed=$(($(date +%s) + total_runs + today_perfect_runs))
        local selected_index=$((random_seed % ${#eligible_milestones[@]}))
        local selected_milestone="${eligible_milestones[$selected_index]}"

        # Split milestone key and message
        IFS=':' read -r milestone_key milestone_message <<< "$selected_milestone"

        # Mark as shown today and display
        mark_milestone_shown_today "$milestone_key"
        echo -e "${PURPLE}🎊 MILESTONE: $milestone_message${NC}"
        echo -e "    ${FADED}legendary achievement unlocked in your coding odyssey${NC}"

        # Show archive statistics for significant milestones
        show_archive_stats
    fi
}

# Function to migrate existing stats files to archive directory
migrate_stats_to_archive() {
    if [ ! -d "$SISYPHUS_DIR" ]; then
        return
    fi

    # Check if there are any .sp-stats files in the main sisyphus directory
    local old_stats_files=($(ls -1 "$SISYPHUS_DIR"/*.sp-stats 2>/dev/null))

    if [ ${#old_stats_files[@]} -gt 0 ]; then
        # Ensure archive directory exists
        mkdir -p "$SISYPHUS_ARCHIVE_DIR"

        # Move all .sp-stats files to archive
        for file in "${old_stats_files[@]}"; do
            mv "$file" "$SISYPHUS_ARCHIVE_DIR/"
        done

        echo -e "${GRAY}📁 Migrated ${#old_stats_files[@]} stats files to archive directory${NC}"
    fi
}

# Function to cleanup old archive files (keep last 30 runs)
cleanup_archive() {
    if [ ! -d "$SISYPHUS_ARCHIVE_DIR" ]; then
        return
    fi

    # Count .sp-stats files
    local file_count=$(ls -1 "$SISYPHUS_ARCHIVE_DIR"/*.sp-stats 2>/dev/null | wc -l)

    if [ "${file_count:-0}" -gt 30 ]; then
        # Remove oldest files (keep newest 30)
        ls -1t "$SISYPHUS_ARCHIVE_DIR"/*.sp-stats 2>/dev/null | tail -n +31 | xargs rm -f
        echo -e "${GRAY}🗑️  Cleaned up old archive files (kept latest 30 runs)${NC}"
    fi
}

# Function to view historical data (for future CLI commands)
view_sisyphus_history() {
    local count=${1:-10}

    if [ ! -d "$SISYPHUS_DIR" ]; then
        echo -e "${RED}🔴 No Sisyphus archive found${NC}"
        return 1
    fi

    echo -e "${PURPLE}🏛️ Sisyphus Progress History (Last $count Runs)${NC}"

    local files=($(ls -1 "$SISYPHUS_DIR"/*.sp-stats 2>/dev/null | head -$count))
    local run_number=1

    for file in "${files[@]}"; do
        local timestamp=$(grep "TIMESTAMP=" "$file" | cut -d'=' -f2 | tr -d '\n\r')
        local errors=$(grep "ERRORS=" "$file" | cut -d'=' -f2 | tr -d '\n\r' | tr -d ' ')
        local passes=$(grep "PASSES=" "$file" | cut -d'=' -f2 | tr -d '\n\r' | tr -d ' ')
        local streak=$(grep "IMPROVEMENT_STREAK=" "$file" | cut -d'=' -f2 | tr -d '\n\r' | tr -d ' ')
        local challenge=$(grep "CURRENT_CHALLENGE=" "$file" | cut -d'=' -f2 | tr -d '\n\r')
        local efficiency=$(grep "EFFICIENCY_RATIO=" "$file" | cut -d'=' -f2 | tr -d '\n\r' | tr -d ' ')

        # Status indicator
        local status="🔴"
        if [ "$errors" -eq 0 ]; then
            status="🟢"
        elif [ "$errors" -le 3 ]; then
            status="🟡"
        fi

        echo -e "${CYAN}Run #$run_number${NC} - $timestamp $status"
        echo -e "  Issues: $errors | Tests: $passes | Streak: ${streak:-0} | Efficiency: ${efficiency}x"
        if [ -n "$challenge" ] && [ "$challenge" != "" ]; then
            echo -e "  Challenge: $challenge"
        fi
        echo ""
        run_number=$((run_number + 1))
    done
}

# Function to show archive statistics
show_archive_stats() {
    if [ ! -d "$SISYPHUS_DIR" ]; then
        return
    fi

    local files=($(ls -1 "$SISYPHUS_DIR"/*.sp-stats 2>/dev/null))
    local total_runs=${#files[@]}
    local perfect_runs=0
    local total_tests=0
    local max_efficiency=0

    for file in "${files[@]}"; do
        local errors=$(grep "ERRORS=" "$file" | cut -d'=' -f2 | tr -d '\n\r' | tr -d ' ')
        local passes=$(grep "PASSES=" "$file" | cut -d'=' -f2 | tr -d '\n\r' | tr -d ' ')
        local efficiency=$(grep "EFFICIENCY_RATIO=" "$file" | cut -d'=' -f2 | tr -d '\n\r' | tr -d ' ')

        if [ -n "$errors" ] && [[ "$errors" =~ ^[0-9]+$ ]] && [ "${errors:-1}" -eq 0 ]; then
            perfect_runs=$((perfect_runs + 1))
        fi

        if [ -n "$passes" ] && [[ "$passes" =~ ^[0-9]+$ ]]; then
            total_tests=$((total_tests + passes))
        fi

        if [ -n "$efficiency" ] && [[ "$efficiency" =~ ^[0-9]+$ ]] && [ "${efficiency:-0}" -gt "${max_efficiency:-0}" ]; then
            max_efficiency=$efficiency
        fi
    done

    if [ "${total_runs:-0}" -gt 0 ]; then
        local success_rate=$((perfect_runs * 100 / total_runs))
        echo -e "${PURPLE}📊 LEGENDARY STATISTICS${NC}"
        echo -e "  🏛️ Total Runs: $total_runs"
        echo -e "  💎 Perfect Runs: $perfect_runs ($success_rate%)"
        echo -e "  ✨ Peak Efficiency: ${max_efficiency}x"
        echo -e "  🧪 Total Tests Passed: $total_tests"
    fi
}

# Function to write current stats to archive system
write_current_stats() {
    local total_errors="$1"
    local total_passes="$2"
    local total_failures="$3"
    local compile_errors="$4"
    local runtime_errors="$5"
    local efficiency_ratio="$6"
    local pure_test_time="$7"
    local improvement_streak="$8"
    local current_challenge="$9"
    local timestamp=$(date '+%Y-%m-%d %H:%M:%S')

    # Ensure pure_test_time has leading zero if needed
    if [[ "$pure_test_time" =~ ^\. ]]; then
        pure_test_time="0$pure_test_time"
    fi

    # Create archive directory if it doesn't exist
    mkdir -p "$SISYPHUS_DIR"

    # Generate timestamped filename
    local stats_file=$(generate_stats_filename)

    cat > "$stats_file" << EOF
# Sisyphus Test Progress Archive Entry
# Last updated: $timestamp
ERRORS=$total_errors
PASSES=$total_passes
FAILURES=$total_failures
COMPILE_ERRORS=$compile_errors
RUNTIME_ERRORS=$runtime_errors
EFFICIENCY_RATIO=$efficiency_ratio
PURE_TEST_TIME=$pure_test_time
IMPROVEMENT_STREAK=$improvement_streak
CURRENT_CHALLENGE=$current_challenge
TIMESTAMP=$timestamp
EOF

    # Migrate existing stats files to archive directory (one-time migration)
    migrate_stats_to_archive

    # Cleanup old files
    cleanup_archive

    # Update lifetime legend stats
        local is_perfect_run="false"
        if [ "$total_errors" -eq 0 ]; then
            is_perfect_run="true"
        fi
        # Calculate final XP: apply bulk mode reduction first, then penalties
        local final_xp=$TOTAL_SESSION_XP
        # Apply bulk mode reduction first (makes failures more punishing)
        if [ "$SISYPHUS_FULL_TEST_MODE" = "1" ] && [ "$SUCCESSFULLY_RUN_FILES" -gt 0 ]; then
            final_xp=$((final_xp / SUCCESSFULLY_RUN_FILES))
        fi
        # Then apply failure penalties
        if [ "$TOTAL_BULK_FAILURES" -gt 0 ]; then
            final_xp=$((final_xp > BULK_FAILURE_PENALTY ? final_xp - BULK_FAILURE_PENALTY : 0))
        fi

        # Debug XP calculation
        echo -e "${GRAY}[DEBUG] Session XP: $TOTAL_SESSION_XP, Bulk mode: 1/$SUCCESSFULLY_RUN_FILES, Failures: $TOTAL_BULK_FAILURES, Penalty: $BULK_FAILURE_PENALTY, Final: $final_xp${NC}" >&2
        # Temporarily set TOTAL_SESSION_XP for legend update
        local saved_session_xp=$TOTAL_SESSION_XP
        TOTAL_SESSION_XP=$final_xp
        update_legend_stats "$RUNTIME_ERRORS" "$COMPILE_ERRORS" "$FAILED_INDIVIDUAL_TESTS" "$improvement_streak" "$is_perfect_run" "$CURRENT_CHALLENGE" "$final_xp"
        # Restore original value
        TOTAL_SESSION_XP=$saved_session_xp
}

# Function to show progress comparison
show_progress() {
    local curr_errors="$1"
    local curr_passes="$2"
    local curr_failures="$3"
    local curr_compile_errors="$4"
    local curr_runtime_errors="$5"
    local curr_efficiency_ratio="$6"
    local curr_pure_test_time="$7"
    local prev_errors="$8"
    local prev_passes="$9"
    local prev_failures="${10}"
    local prev_compile_errors="${11}"
    local prev_runtime_errors="${12}"
    local prev_efficiency_ratio="${13}"
    local prev_pure_test_time="${14}"
    local prev_improvement_streak="${15}"
    local prev_current_challenge="${16}"
    local prev_timestamp="${17}"
    echo ""
    echo -e "${PURPLE}📈 Sisyphus Progress Since Last Run${NC}"
    echo -e "Last run: ${YELLOW}$prev_timestamp${NC}"
    echo ""

    # Calculate changes
    local error_change=$((curr_errors - prev_errors))
    local pass_change=$((curr_passes - prev_passes))
    local failure_change=$((curr_failures - prev_failures))
    local compile_change=$((curr_compile_errors - prev_compile_errors))
    local runtime_change=$((curr_runtime_errors - prev_runtime_errors))

    # Helper function to format change
        format_change() {
            local change=$1
            local label=$2
            if [ "$change" -gt 0 ]; then
                echo -e "${RED}+$change $label${NC}"
            elif [ "$change" -lt 0 ]; then
                echo -e "${GREEN}$change $label${NC}"  # negative is good for errors
            else
                echo -e "${YELLOW}±0 $label${NC}"
            fi
        }

        # Helper function to format change for passes (opposite logic - more passes is good)
        format_pass_change() {
            local change=$1
            local label=$2
            if [ "$change" -gt 0 ]; then
                echo -e "${GREEN}+$change $label${NC}"  # positive is good for passes
            elif [ "$change" -lt 0 ]; then
                echo -e "${RED}$change $label${NC}"    # negative is bad for passes
            else
                echo -e "${YELLOW}±0 $label${NC}"
            fi
        }

        echo -e "Total File Errors:   ${YELLOW}$prev_errors${NC} → ${YELLOW}$curr_errors${NC} ($(format_change $error_change "errors"))"
        echo -e "Individual Passes:   ${YELLOW}$prev_passes${NC} → ${YELLOW}$curr_passes${NC} ($(format_pass_change $pass_change "tests"))"
        echo -e "Individual Failures: ${YELLOW}$prev_failures${NC} → ${YELLOW}$curr_failures${NC} ($(format_change $failure_change "tests"))"
        echo -e "Compile Errors:      ${YELLOW}$prev_compile_errors${NC} → ${YELLOW}$curr_compile_errors${NC} ($(format_change $compile_change "files"))"
        echo -e "Runtime Errors:      ${YELLOW}$prev_runtime_errors${NC} → ${YELLOW}$curr_runtime_errors${NC} ($(format_change $runtime_change "files"))"

    # Overall progress indicator - avoid double-counting test failures
    # curr_errors represents files with test failures, curr_failures represents individual test failures
    # We should only count one or the other, not both, to avoid double-counting the same issue
    local total_curr_issues=$((curr_failures + curr_compile_errors + curr_runtime_errors))
    local total_prev_issues=$((prev_failures + prev_compile_errors + prev_runtime_errors))
    local total_change=$((total_curr_issues - total_prev_issues))

    echo ""
    if [ "$total_change" -lt 0 ]; then
        echo -e "${GREEN}🎯 Overall Progress: IMPROVING${NC} (${GREEN}$total_change issues${NC})"
    elif [ "$total_change" -gt 0 ]; then
        echo -e "${RED}⚠️  Overall Progress: REGRESSING${NC} (${RED}+$total_change issues${NC})"
    else
        echo -e "${YELLOW}📊 Overall Progress: STABLE${NC} (no change in issues)"
    fi

    # Success rate comparison
    if [ "${curr_passes:-0}" -gt 0 ] || [ "${prev_passes:-0}" -gt 0 ]; then
        local curr_total_tests=$((curr_passes + curr_failures))
        local prev_total_tests=$((prev_passes + prev_failures))

        if [ "${curr_total_tests:-0}" -gt 0 ] && [ "${prev_total_tests:-0}" -gt 0 ]; then
            local curr_rate=$(echo "scale=1; $curr_passes * 100 / $curr_total_tests" | bc -l)
            local prev_rate=$(echo "scale=1; $prev_passes * 100 / $prev_total_tests" | bc -l)
            local rate_change=$(echo "scale=1; $curr_rate - $prev_rate" | bc -l)

            echo -e "Success Rate:        ${YELLOW}$(printf "%.1f" $prev_rate)%${NC} → ${YELLOW}$(printf "%.1f" $curr_rate)%${NC}"

            if (( $(echo "$rate_change > 0" | bc -l) )); then
                echo -e "                     ${GREEN}▲ +$(printf "%.1f" $rate_change)% improvement${NC}"
            elif (( $(echo "$rate_change < 0" | bc -l) )); then
                echo -e "                     ${RED}▼ $(printf "%.1f" $rate_change)% regression${NC}"
            fi
        fi
    fi

    # Performance comparison - only if we have valid previous data
    # Validate that all values are proper numbers first
    if [[ "$prev_pure_test_time" =~ ^[0-9]+\.[0-9]+$ ]] && [[ "$curr_pure_test_time" =~ ^[0-9]+\.[0-9]+$ ]] && \
       [[ "$prev_efficiency_ratio" =~ ^[0-9]+$ ]] && [[ "$curr_efficiency_ratio" =~ ^[0-9]+$ ]] && \
       [ "$prev_pure_test_time" != "0.000000" ] && [ "$prev_efficiency_ratio" != "0" ] && (( $(echo "$curr_pure_test_time > 0" | bc -l) )); then

        local time_change=$(echo "scale=6; $curr_pure_test_time - $prev_pure_test_time" | bc -l)
        local efficiency_change=$((curr_efficiency_ratio - prev_efficiency_ratio))

        echo ""
        echo -e "🔥 Performance Changes:"
        echo -e "Pure Test Time:      ${YELLOW}$(printf "%.6f" "$prev_pure_test_time")s${NC} → ${YELLOW}$(printf "%.6f" "$curr_pure_test_time")s${NC}"

        if (( $(echo "$time_change < 0" | bc -l) )); then
            local abs_change=$(echo "$time_change" | sed 's/^-//')
            echo -e "                     ${GREEN}▼ ${abs_change}s faster${NC}"
        elif (( $(echo "$time_change > 0" | bc -l) )); then
            echo -e "                     ${RED}▲ +$(printf "%.6f" $time_change)s slower${NC}"
        else
            echo -e "                     ${YELLOW}± No change${NC}"
        fi

        echo -e "Code Efficiency:     ${YELLOW}${prev_efficiency_ratio}x${NC} → ${YELLOW}${curr_efficiency_ratio}x${NC}"

        if [ "$efficiency_change" -gt 0 ]; then
            echo -e "                     ${GREEN}▲ +${efficiency_change}x more efficient${NC}"
        elif [ "${efficiency_change:-0}" -lt 0 ]; then
            echo -e "                     ${RED}▼ ${efficiency_change}x less efficient${NC}"
        else
            echo -e "                     ${YELLOW}± No change${NC}"
        fi
    fi
}

# Build test dependencies
echo -e "${BLUE}🔧 Building test dependencies...${NC}"
build_start=$(date +%s.%N)
if ! make -s always > /dev/null 2>&1; then
    echo -e "${RED}🔴 Failed to build test dependencies${NC}"
    exit 1
fi
build_end=$(date +%s.%N)
build_time=$(echo "$build_end - $build_start" | bc -l)
echo -e "${BOLD_WHITE}   Build completed in $(printf "%.6f" $build_time) seconds${NC}"

# Counter variables for files
TOTAL_FILES=0
PASSED_FILES=0
FAILED_FILES=()
COMPILE_ERROR_FILES=()
RUNTIME_ERROR_FILES=()
SUCCESSFULLY_RUN_FILES=0  # Files that ran tests (no compile/runtime errors)

# Counter variables for individual tests
TOTAL_INDIVIDUAL_TESTS=0
PASSED_INDIVIDUAL_TESTS=0
FAILED_INDIVIDUAL_TESTS=0

# Error tracking
COMPILE_ERRORS=0
RUNTIME_ERRORS=0

# Timing variables
TOTAL_TEST_TIME=0
TOTAL_SHELL_OVERHEAD=0

# XP tracking for overall test run (will be calculated from project XP difference)
TOTAL_SESSION_XP=0
SESSION_XP_FROM_TESTS=0
SESSION_XP_FROM_COMBOS=0
SESSION_XP_FROM_ACHIEVEMENTS=0
TOTAL_BULK_FAILURES=0
BULK_FAILURE_PENALTY=0

# Read starting project XP to calculate session XP later
STARTING_LEGEND_LINE="$(read_legend_stats)"
IFS='|' read -r _start_runs _start_quests _start_hydra _start_blueprint _start_oracle _start_divine _start_longest _start_perfect _start_first_quest STARTING_PROJECT_XP _start_level <<< "$STARTING_LEGEND_LINE"

# Debug XP initialization
echo -e "${GRAY}[DEBUG] XP tracking initialized. Bulk mode: Dynamic (1/N where N = successfully run files), Starting Project XP: $STARTING_PROJECT_XP${NC}" >&2

# Function to extract test counts from output
extract_test_counts() {
    local output="$1"

    # Look for the FINAL Sisyphus test summary line: "🧪 Test Functions Executed: 6 | 🏆 Won: 4 | 💀 Lost: 2"
    # Use tail to get the last occurrence in case there are multiple summary lines
    local total=$(echo "$output" | grep -o "Test Functions Executed: [0-9]*" | tail -1 | grep -o "[0-9]*" || echo "0")
    local passed=$(echo "$output" | grep -o "🏆 Won: [0-9]*" | tail -1 | grep -o "[0-9]*" || echo "0")
    local failed=$(echo "$output" | grep -o "💀 Lost: [0-9]*" | tail -1 | grep -o "[0-9]*" || echo "0")

    # Return as space-separated values
    echo "$total $passed $failed"
}


# Function to detect error type from output
detect_error_type() {
    local output="$1"
    local exit_code="$2"

    # Check for compilation errors - but exclude log output patterns
    # Compilation errors typically have format: "file.c:line:col: error: message"
    # Log output has format: "[ERROR]" or "❌ This is an ERROR message"
    if echo "$output" | grep -q -E "(^[^:]*:[0-9]+:[0-9]*: error:|fatal error:|undefined reference|ld:.*error|collect2:)" && \
       ! echo "$output" | grep -q -E "(\[ERROR\]|\[FATAL\]|❌|💀|🚨)" && \
       ! echo "$output" | grep -q "logging" && \
       ! echo "$output" | grep -q "COLOSSEUM"; then
        echo "COMPILE_ERROR"
        return
    fi

    # Check for test failures BEFORE checking for runtime errors
    # Look for the Sisyphus test output patterns with more robust detection
    if echo "$output" | grep -q "FINAL COLOSSEUM STATS"; then
        # It's a test run that completed - check the final summary line more carefully
        # Look for the last occurrence of the test summary to handle verbose output
        local final_failed=$(echo "$output" | grep "💀 Lost:" | tail -1 | grep -o "💀 Lost: [0-9]*" | grep -o "[0-9]*" || echo "0")
        local final_passed=$(echo "$output" | grep "🏆 Won:" | tail -1 | grep -o "🏆 Won: [0-9]*" | grep -o "[0-9]*" || echo "0")

        # If we found valid counts, use them
        if [ -n "$final_failed" ] && [ -n "$final_passed" ] && [ "$final_failed" != "0" ]; then
            echo "TEST_FAILURE"
            return
        elif [ -n "$final_passed" ] && [ "$final_passed" != "0" ]; then
            # Also check for explicit success indicators in the final output
            if echo "$output" | tail -20 | grep -q -E "(return 0|SUCCESS|All.*passed)"; then
                echo "SUCCESS"
                return
            elif [ "$exit_code" -eq 0 ]; then
                echo "SUCCESS"
                return
            fi
        fi

        # Fallback: if we have a complete test run but unclear results, check exit code
        if [ "$exit_code" -eq 0 ]; then
            echo "SUCCESS"
            return
        else
            echo "TEST_FAILURE"
            return
        fi
    fi

    # Check for runtime errors (actual crashes)
    if echo "$output" | grep -q -E "(Segmentation fault|core dumped|Aborted|Bus error|Floating point exception)"; then
        echo "RUNTIME_ERROR"
        return
    fi

    # If exit code is non-zero but we can't classify it
    if [ "$exit_code" -ne 0 ]; then
        echo "RUNTIME_ERROR"
        return
    fi

    echo "SUCCESS"
}



# Function to run a test
run_test() {
    local test_name="$1"
    local make_target="$2"

    echo -ne "${BLUE}📋 Running $test_name...${NC}"

    TOTAL_FILES=$((TOTAL_FILES + 1))

    # Capture start time for full process
    process_start=$(date +%s.%N)

    # Run the test and capture both exit code and output
    test_output=$(make -s "$make_target" 2>&1)
    exit_code=$?

    process_end=$(date +%s.%N)
    process_elapsed=$(echo "$process_end - $process_start" | bc -l)

    # Detect error type
    error_type=$(detect_error_type "$test_output" "$exit_code")

    # Debug output for troubleshooting
    if [ "$test_name" = "Logging Basic Operations" ]; then
        echo -e "${GRAY}[DEBUG] Test: $test_name | Exit Code: $exit_code | Detected Type: $error_type${NC}" >&2
        echo -e "${GRAY}[DEBUG] Final summary line: $(echo "$test_output" | grep "💀 Lost:" | tail -1)${NC}" >&2
        echo -e "${GRAY}[DEBUG] Has COLOSSEUM: $(echo "$test_output" | grep -q "FINAL COLOSSEUM STATS" && echo "YES" || echo "NO")${NC}" >&2
    fi

    case "$error_type" in
        "SUCCESS")
            # Extract timing from the test output if available (format: ⏱️  Total Test Time: 0.000155 seconds)
            actual_test_time=$(echo "$test_output" | grep -o "Total Test Time: [0-9]*\.[0-9]* seconds" | grep -o "[0-9]\+\.[0-9]\+" || echo "0.000000")
            # Ensure proper formatting with leading zero if needed
            if [[ "$actual_test_time" =~ ^\. ]]; then
                actual_test_time="0$actual_test_time"
            fi
            shell_overhead=$(echo "$process_elapsed - $actual_test_time" | bc -l)

            # Extract individual test counts
            counts=$(extract_test_counts "$test_output")
            read -r file_total file_passed file_failed <<< "$counts"

            # Add to global counters
            TOTAL_INDIVIDUAL_TESTS=$((TOTAL_INDIVIDUAL_TESTS + file_total))
            PASSED_INDIVIDUAL_TESTS=$((PASSED_INDIVIDUAL_TESTS + file_passed))
            FAILED_INDIVIDUAL_TESTS=$((FAILED_INDIVIDUAL_TESTS + file_failed))

            # Extract Suite XP from the progress bar line: "🎯 Suite XP [...] 928/2100 (Level 1)"
            session_xp=$(echo "$test_output" | grep -o "Suite XP \[.*\] [0-9]*/[0-9]*" | grep -o "[0-9]*/" | grep -o "[0-9]*" | head -1 || echo "0")
            # Extract XP breakdowns from: "💰 XP from tests: 935, XP from combos: 213, XP from achievements: 80"
            tests_xp=$(echo "$test_output" | grep -o "XP from tests: [0-9]*" | grep -o "[0-9]*" | tail -1 || echo "0")
            combos_xp=$(echo "$test_output" | grep -o "XP from combos: [0-9]*" | grep -o "[0-9]*" | tail -1 || echo "0")
            achievements_xp=$(echo "$test_output" | grep -o "XP from achievements: [0-9]*" | grep -o "[0-9]*" | tail -1 || echo "0")

            # Add to session totals
            TOTAL_SESSION_XP=$((TOTAL_SESSION_XP + session_xp))
            SESSION_XP_FROM_TESTS=$((SESSION_XP_FROM_TESTS + tests_xp))
            SESSION_XP_FROM_COMBOS=$((SESSION_XP_FROM_COMBOS + combos_xp))
            SESSION_XP_FROM_ACHIEVEMENTS=$((SESSION_XP_FROM_ACHIEVEMENTS + achievements_xp))

            TOTAL_TEST_TIME=$(echo "$TOTAL_TEST_TIME + $actual_test_time" | bc -l)
            TOTAL_SHELL_OVERHEAD=$(echo "$TOTAL_SHELL_OVERHEAD + $shell_overhead" | bc -l)

            echo -e " ${BOLD_WHITE}(Test: $(printf "%.6f" $actual_test_time)s | Shell: $(printf "%.3f" $shell_overhead)s)${NC}"
            echo -e "  ${GREEN}✅ $test_name: All $file_total tests passed${NC}"
            PASSED_FILES=$((PASSED_FILES + 1))
            SUCCESSFULLY_RUN_FILES=$((SUCCESSFULLY_RUN_FILES + 1))
            ;;

        "TEST_FAILURE")
            # Extract timing if available (format: ⏱️  Total Test Time: 0.000155 seconds)
            actual_test_time=$(echo "$test_output" | grep -o "Total Test Time: [0-9]*\.[0-9]* seconds" | grep -o "[0-9]\+\.[0-9]\+" || echo "0.000000")
            # Ensure proper formatting with leading zero if needed
            if [[ "$actual_test_time" =~ ^\. ]]; then
                actual_test_time="0$actual_test_time"
            fi
            shell_overhead=$(echo "$process_elapsed - $actual_test_time" | bc -l)

            # Extract individual test counts
            counts=$(extract_test_counts "$test_output")
            read -r file_total file_passed file_failed <<< "$counts"

            # Add to global counters
            TOTAL_INDIVIDUAL_TESTS=$((TOTAL_INDIVIDUAL_TESTS + file_total))
            PASSED_INDIVIDUAL_TESTS=$((PASSED_INDIVIDUAL_TESTS + file_passed))
            FAILED_INDIVIDUAL_TESTS=$((FAILED_INDIVIDUAL_TESTS + file_failed))

            # Extract Suite XP from the progress bar line: "🎯 Suite XP [...] 928/2100 (Level 1)"
            session_xp=$(echo "$test_output" | grep -o "Suite XP \[.*\] [0-9]*/[0-9]*" | grep -o "[0-9]*/" | grep -o "[0-9]*" | head -1 || echo "0")
            # Extract XP breakdowns from: "💰 XP from tests: 935, XP from combos: 213, XP from achievements: 80"
            tests_xp=$(echo "$test_output" | grep -o "XP from tests: [0-9]*" | grep -o "[0-9]*" | tail -1 || echo "0")
            combos_xp=$(echo "$test_output" | grep -o "XP from combos: [0-9]*" | grep -o "[0-9]*" | tail -1 || echo "0")
            achievements_xp=$(echo "$test_output" | grep -o "XP from achievements: [0-9]*" | grep -o "[0-9]*" | tail -1 || echo "0")

            # Add to session totals
            TOTAL_SESSION_XP=$((TOTAL_SESSION_XP + session_xp))
            SESSION_XP_FROM_TESTS=$((SESSION_XP_FROM_TESTS + tests_xp))
            SESSION_XP_FROM_COMBOS=$((SESSION_XP_FROM_COMBOS + combos_xp))
            SESSION_XP_FROM_ACHIEVEMENTS=$((SESSION_XP_FROM_ACHIEVEMENTS + achievements_xp))

            TOTAL_TEST_TIME=$(echo "$TOTAL_TEST_TIME + $actual_test_time" | bc -l)
            TOTAL_SHELL_OVERHEAD=$(echo "$TOTAL_SHELL_OVERHEAD + $shell_overhead" | bc -l)

            echo -e " ${BOLD_WHITE}(Test: $(printf "%.6f" $actual_test_time)s | Shell: $(printf "%.3f" $shell_overhead)s)${NC}"
            echo -e "  ${YELLOW}⚠️  $test_name: $file_passed/$file_total tests passed, $file_failed failed${NC}"
            FAILED_FILES+=("$test_name:$make_target:TEST_FAILURE")
            SUCCESSFULLY_RUN_FILES=$((SUCCESSFULLY_RUN_FILES + 1))
            ;;

        "COMPILE_ERROR")
            TOTAL_SHELL_OVERHEAD=$(echo "$TOTAL_SHELL_OVERHEAD + $process_elapsed" | bc -l)
            echo -e " ${BOLD_WHITE}(Process: $(printf "%.6f" $process_elapsed)s)${NC}"
            echo -e "  ${RED}🔨 $test_name: Compilation error${NC}"
            COMPILE_ERROR_FILES+=("$test_name:$make_target")
            COMPILE_ERRORS=$((COMPILE_ERRORS + 1))
            ;;

        "RUNTIME_ERROR")
            TOTAL_SHELL_OVERHEAD=$(echo "$TOTAL_SHELL_OVERHEAD + $process_elapsed" | bc -l)
            echo -e " ${BOLD_WHITE}(Process: $(printf "%.6f" $process_elapsed)s)${NC}"
            echo -e "  ${ORANGE}💥 $test_name: Runtime error (segfault/crash)${NC}"
            RUNTIME_ERROR_FILES+=("$test_name:$make_target")
            RUNTIME_ERRORS=$((RUNTIME_ERRORS + 1))
            ;;
    esac
}

# Record overall start time
overall_start=$(date +%s.%N)

# Run all tests
echo ""
run_test "Input Basic" "run-test-input-basic"
run_test "Input Advanced" "run-test-input-advanced"
run_test "Input Edge Cases" "run-test-input-edge"
run_test "Input Performance" "run-test-input-performance"
run_test "Input Errors" "run-test-input-errors"
run_test "Input Integration" "run-test-input-integration"
run_test "Draw Basic" "run-test-draw-basic"
run_test "Image Basic" "run-test-image-basic"
run_test "Image Advanced" "run-test-image-advanced"

# Calculate overall execution time
overall_end=$(date +%s.%N)
overall_time=$(echo "$overall_end - $overall_start" | bc -l)

# Calculate total file errors
TOTAL_FILE_ERRORS=$((COMPILE_ERRORS + RUNTIME_ERRORS + ${#FAILED_FILES[@]}))

# Ensure TOTAL_TEST_TIME is properly formatted
if [[ "$TOTAL_TEST_TIME" =~ ^\. ]]; then
    TOTAL_TEST_TIME="0$TOTAL_TEST_TIME"
fi

# Calculate efficiency ratio before calling show_progress
# Set bulk failures to total individual test failures across all test files
TOTAL_BULK_FAILURES=$FAILED_INDIVIDUAL_TESTS

# Calculate bulk failure penalty - exponential: -100, -200, -300, etc.
if [ "$TOTAL_BULK_FAILURES" -gt 0 ]; then
    for ((i=1; i<=TOTAL_BULK_FAILURES; i++)); do
        penalty=$((i * 100))
        BULK_FAILURE_PENALTY=$((BULK_FAILURE_PENALTY + penalty))
    done
fi

# Determine efficiency ratio BEFORE writing stats
EFFICIENCY_RATIO=0
if (( $(echo "$TOTAL_TEST_TIME > 0" | bc -l) )); then
    EFFICIENCY_RATIO=$(echo "scale=0; $TOTAL_SHELL_OVERHEAD / $TOTAL_TEST_TIME" | bc -l)
fi
# Generate current challenge from failed tests
ALL_FAILED_TESTS=()
for failed in "${FAILED_FILES[@]}"; do
    ALL_FAILED_TESTS+=("$failed")
done
for compile_error in "${COMPILE_ERROR_FILES[@]}"; do
    ALL_FAILED_TESTS+=("$compile_error:COMPILE_ERROR")
done
for runtime_error in "${RUNTIME_ERROR_FILES[@]}"; do
    ALL_FAILED_TESTS+=("$runtime_error:RUNTIME_ERROR")
done

# Read previous challenge
stats_line="$(read_previous_stats)"
IFS='|' read -r prev_errors_temp prev_passes_temp prev_failures_temp prev_compile_errors_temp prev_runtime_errors_temp prev_efficiency_ratio_temp prev_pure_test_time_temp prev_streak_temp prev_challenge_temp prev_timestamp_temp <<< "$stats_line"

# Generate new challenge if needed or if previous challenge is resolved
CURRENT_CHALLENGE="$prev_challenge_temp"
if [ "$CURRENT_CHALLENGE" = "none" ] || [ ${#ALL_FAILED_TESTS[@]} -eq 0 ]; then
    CURRENT_CHALLENGE=$(generate_current_challenge "${ALL_FAILED_TESTS[@]}")
else
    # Check if current challenge is still valid (still in failed tests)
    challenge_still_valid=false
    for failed_test in "${ALL_FAILED_TESTS[@]}"; do
        if echo "$failed_test" | grep -q "$CURRENT_CHALLENGE"; then
            challenge_still_valid=true
            break
        fi
    done

    # If current challenge is resolved, generate new one
    if [ "$challenge_still_valid" = false ]; then
        CURRENT_CHALLENGE=$(generate_current_challenge "${ALL_FAILED_TESTS[@]}")
    fi
fi

# Calculate streak and challenge for current run
IMPROVEMENT_STREAK=0
CURRENT_CHALLENGE=""

# Check if we have archive or legacy file
if [ -d "$SISYPHUS_DIR" ] || [ -f "$SISYPHUS_FILE" ]; then
    prev_stats="$(read_previous_stats)"
    IFS='|' read -r prev_errors_temp prev_passes_temp prev_failures_temp prev_compile_errors_temp prev_runtime_errors_temp prev_efficiency_ratio_temp prev_pure_test_time_temp prev_streak_temp prev_challenge_temp prev_timestamp_temp <<< "$prev_stats"

    # Only calculate if we have valid previous data
    if [ "$prev_timestamp_temp" != "never" ]; then
        prev_total_issues=$((prev_errors_temp + prev_failures_temp + prev_compile_errors_temp + prev_runtime_errors_temp))
        curr_total_issues=$((TOTAL_FILE_ERRORS + FAILED_INDIVIDUAL_TESTS + COMPILE_ERRORS + RUNTIME_ERRORS))

        if [ "$curr_total_issues" -lt "$prev_total_issues" ]; then
            IMPROVEMENT_STREAK=$((prev_streak_temp + 1))
        elif [ "$curr_total_issues" -gt "$prev_total_issues" ]; then
            IMPROVEMENT_STREAK=0
        else
            IMPROVEMENT_STREAK=$prev_streak_temp
        fi
    fi
fi

# Set current challenge based on remaining issues (Epic Mythological Themes)
if [ "$COMPILE_ERRORS" -gt 0 ]; then
    CURRENT_CHALLENGE="🏗️ Daedalus' Workshop Crisis (${COMPILE_ERRORS} blueprint flaws - undefined symbols, missing headers, linker failures)"
elif [ "$RUNTIME_ERRORS" -gt 0 ]; then
    CURRENT_CHALLENGE="🐲 Hydra Beasts Rampage (${RUNTIME_ERRORS} runtime errors - segfaults, buffer overflows, null dereferences)"
elif [ ${#FAILED_FILES[@]} -gt 0 ]; then
    if [ ${#FAILED_FILES[@]} -eq 12 ]; then
        CURRENT_CHALLENGE="🌊 Poseidon's Twelve Trials (${#FAILED_FILES[@]} assertion storms - systematic logic validation required)"
    elif [ ${#FAILED_FILES[@]} -eq 1 ]; then
        CURRENT_CHALLENGE="👑 Minos' Final Requiem (${#FAILED_FILES[@]} labyrinth exit - one critical test failure blocking ascension)"
    else
        CURRENT_CHALLENGE="🏛️ Minotaur's Labyrinth (${#FAILED_FILES[@]} twisted corridors - failed assertions, logic errors, boundary conditions)"
    fi
elif [ "$TOTAL_FILE_ERRORS" -gt 0 ]; then
    CURRENT_CHALLENGE="✨ Zeus's Divine Judgment (${TOTAL_FILE_ERRORS} compilation warnings elevated to olympian standards)"
else
    CURRENT_CHALLENGE="🏛️ Mount Olympus Achieved - All tests pass, Daedalus' labyrinth conquered"
fi

# Read previous stats once and pass to both functions
PREV_STATS_LINE="$(read_previous_stats)"
IFS='|' read -r PREV_ERRORS PREV_PASSES PREV_FAILURES PREV_COMPILE_ERRORS PREV_RUNTIME_ERRORS PREV_EFFICIENCY_RATIO PREV_PURE_TEST_TIME PREV_IMPROVEMENT_STREAK PREV_CURRENT_CHALLENGE PREV_TIMESTAMP <<< "$PREV_STATS_LINE"

# Show progress comparison BEFORE writing current stats
show_progress "$TOTAL_FILE_ERRORS" "$PASSED_INDIVIDUAL_TESTS" "$FAILED_INDIVIDUAL_TESTS" "$COMPILE_ERRORS" "$RUNTIME_ERRORS" "$EFFICIENCY_RATIO" "$TOTAL_TEST_TIME" "$PREV_ERRORS" "$PREV_PASSES" "$PREV_FAILURES" "$PREV_COMPILE_ERRORS" "$PREV_RUNTIME_ERRORS" "$PREV_EFFICIENCY_RATIO" "$PREV_PURE_TEST_TIME" "$PREV_IMPROVEMENT_STREAK" "$PREV_CURRENT_CHALLENGE" "$PREV_TIMESTAMP"

# Write stats AFTER showing progress comparison
write_current_stats "$TOTAL_FILE_ERRORS" "$PASSED_INDIVIDUAL_TESTS" "$FAILED_INDIVIDUAL_TESTS" "$COMPILE_ERRORS" "$RUNTIME_ERRORS" "$EFFICIENCY_RATIO" "$TOTAL_TEST_TIME" "$IMPROVEMENT_STREAK" "$CURRENT_CHALLENGE"

# Now calculate actual session XP as the difference between ending and starting project XP
ENDING_LEGEND_LINE="$(read_legend_stats)"
IFS='|' read -r _end_runs _end_quests _end_hydra _end_blueprint _end_oracle _end_divine _end_longest _end_perfect _end_first_quest ENDING_PROJECT_XP _end_level <<< "$ENDING_LEGEND_LINE"
TOTAL_SESSION_XP=$((ENDING_PROJECT_XP - STARTING_PROJECT_XP))

# Store original XP totals for display (never modify the originals)
original_session_xp=$TOTAL_SESSION_XP

# Debug final totals before display
echo -e "${GRAY}[DEBUG] Final totals - Starting XP: $STARTING_PROJECT_XP, Ending XP: $ENDING_PROJECT_XP, Session XP: $TOTAL_SESSION_XP, Successfully run files: $SUCCESSFULLY_RUN_FILES, Failed tests: $FAILED_INDIVIDUAL_TESTS, Bulk penalty: $BULK_FAILURE_PENALTY${NC}" >&2

# Final Summary
SUCCESSFUL_FILES=$((PASSED_FILES))
FAILED_FILE_COUNT=$((${#FAILED_FILES[@]}))

echo ""
echo -e "${PURPLE}📊 Current Test Results Summary${NC}"
# Read current project level and XP for progress bar
legend_line="$(read_legend_stats)"
IFS='|' read -r total_runs total_quests hydra_beasts blueprint_flaws oracle_mysteries divine_streaks longest_streak perfect_ascensions first_quest total_project_xp project_level <<< "$legend_line"

# Calculate XP progress within current level using C header formula
# XP formula: 1000 + (level^3 * 200) + (level^2 * 600) + (level * 300)
get_xp_for_level() {
    local level=$1
    echo $(( 1000 + (level * level * level * 200) + (level * level * 600) + (level * 300) ))
}

get_current_level_from_xp() {
    local total_xp=$1
    local level=1
    local cumulative_xp=0

    while true; do
        local xp_needed=$(get_xp_for_level $level)
        if [ $((cumulative_xp + xp_needed)) -gt $total_xp ]; then
            break
        fi
        cumulative_xp=$((cumulative_xp + xp_needed))
        level=$((level + 1))
    done
    echo $level
}

get_xp_in_current_level() {
    local total_xp=$1
    local current_level=$2
    local cumulative_xp=0

    for ((i=1; i<current_level; i++)); do
        cumulative_xp=$((cumulative_xp + $(get_xp_for_level $i)))
    done
    echo $((total_xp - cumulative_xp))
}

# Recalculate project level based on total XP using correct formula
actual_project_level=$(get_current_level_from_xp $total_project_xp)
current_level_xp=$(get_xp_in_current_level $total_project_xp $actual_project_level)
xp_needed_for_next=$(get_xp_for_level $actual_project_level)

# Progress bar will be shown later as separator before mythological messages
echo -e "${CYAN}📁 Test File Summary:${NC}"
echo -e "Test Files Run:     ${YELLOW}$TOTAL_FILES${NC}"
echo -e "🟢 Files Passed:    ${GREEN}$SUCCESSFUL_FILES${NC}"
echo -e "🔴 Files w/ Failures: ${YELLOW}$FAILED_FILE_COUNT${NC}"
echo -e "🔨 Compile Errors:  ${RED}$COMPILE_ERRORS${NC}"
echo -e "💥 Runtime Errors:  ${ORANGE}$RUNTIME_ERRORS${NC}"
echo ""
echo -e "${CYAN}🧪 Individual Test Summary:${NC}"
echo -e "Total Tests:    ${YELLOW}$TOTAL_INDIVIDUAL_TESTS${NC}"
echo -e "🟢 Passed:      ${GREEN}$PASSED_INDIVIDUAL_TESTS${NC}"
echo -e "🔴 Failed:      ${RED}$FAILED_INDIVIDUAL_TESTS${NC}"

# Calculate success percentage
if [ "$TOTAL_INDIVIDUAL_TESTS" -gt 0 ]; then
    success_rate=$(echo "scale=1; $PASSED_INDIVIDUAL_TESTS * 100 / $TOTAL_INDIVIDUAL_TESTS" | bc -l)
    echo -e "📈 Success Rate: ${BOLD_WHITE}$(printf "%.1f" $success_rate)%${NC}"
fi

echo ""
echo -e "${CYAN}✨ Performance Analysis:${NC}"
if (( $(echo "$TOTAL_TEST_TIME > 0" | bc -l) )); then
    echo -e "${BOLD_WHITE}⏱️  Pure Test Time:     $(printf "%10.5f" $TOTAL_TEST_TIME) seconds${NC}"
    echo -e "${BOLD_WHITE}🐚 Shell Overhead:     $(printf "%10.3f" $TOTAL_SHELL_OVERHEAD) seconds${NC}"
    echo -e "${BOLD_WHITE}🕒 Total Runtime:      $(printf "%10.4f" $overall_time) seconds${NC}"
    echo -e "${BOLD_WHITE}🚀 Code Efficiency: Your Code is ${GREEN}${EFFICIENCY_RATIO}x${BOLD_WHITE} faster than the Sisyphus Framework!${NC}"
else
    echo -e "${BOLD_WHITE}💥 Runtime Crashes Detected: Tests failed to execute${NC}"
    echo -e "${BOLD_WHITE}🐚 Shell Overhead:     $(printf "%10.3f" $TOTAL_SHELL_OVERHEAD) seconds${NC}"
    echo -e "${BOLD_WHITE}🕒 Total Runtime:      $(printf "%10.4f" $overall_time) seconds${NC}"
    echo -e "${BOLD_WHITE}🔧 Fix segfaults to measure actual test performance${NC}"
fi



# Show detailed error breakdown (keeping the existing detailed error reporting)
if [ ${#COMPILE_ERROR_FILES[@]} -gt 0 ] || [ ${#RUNTIME_ERROR_FILES[@]} -gt 0 ] || [ ${#FAILED_FILES[@]} -gt 0 ]; then
    echo ""
    echo -e "${RED}💥 Issues Found:${NC}"

    if [ ${#COMPILE_ERROR_FILES[@]} -gt 0 ]; then
        echo ""
        echo -e "${RED}🏗️ DAEDALUS' WORKSHOP CRISIS - Blueprint Corruption Detected:${NC}"
        for error_file in "${COMPILE_ERROR_FILES[@]}"; do
            name=$(echo "$error_file" | cut -d':' -f1)
            target=$(echo "$error_file" | cut -d':' -f2)
            echo -e "  ${RED}• $name${NC} - Architectural flaws in the labyrinth design"
            echo -e "    ${GRAY}Common causes: missing headers, undefined symbols, linker dependencies${NC}"
            file_path=$(map_target_to_file_path "$target")
            echo -e "    Debug: ${YELLOW}make $target${NC} | ${CYAN}View Test:${NC} \e]8;;file://$(pwd)/$file_path\e\\Click Here\e]8;;\e\\"
        done
    fi

    if [ ${#RUNTIME_ERROR_FILES[@]} -gt 0 ]; then
        echo ""
        echo -e "${ORANGE}🐲 MYTHICAL BEASTS RAMPAGE - Memory Corruption Unleashed:${NC}"
        for error_file in "${RUNTIME_ERROR_FILES[@]}"; do
            name=$(echo "$error_file" | cut -d':' -f1)
            target=$(echo "$error_file" | cut -d':' -f2)
            echo -e "  ${ORANGE}• $name${NC} - Hydra head strikes: segmentation fault, buffer overflow, or null pointer"
            echo -e "    ${GRAY}Memory beasts detected: check malloc/free pairs, array bounds, pointer initialization${NC}"
            file_path=$(map_target_to_file_path "$target")
            echo -e "    Debug: ${YELLOW}make $target${NC} | ${CYAN}View Test:${NC} \e]8;;file://$(pwd)/$file_path\e\\Click Here\e]8;;\e\\"
        done
    fi

    if [ ${#FAILED_FILES[@]} -gt 0 ]; then
        echo ""
        echo -e "${YELLOW}🏛️  MINOTAUR'S LABYRINTH - Logic Paths Blocked:${NC}"
        for failed in "${FAILED_FILES[@]}"; do
            name=$(echo "$failed" | cut -d':' -f1)
            target=$(echo "$failed" | cut -d':' -f2)
            echo -e "  ${YELLOW}• $name${NC} - The corridor is blocked: logical fallacies, circular reasoning, or paradoxes"
            echo -e "    ${GRAY}Navigate maze: check expected vs actual values, boundary conditions, algorithm correctness${NC}"
            file_path=$(map_target_to_file_path "$target")
            echo -e "    Explore: ${YELLOW}make $target${NC} | ${CYAN}View Test:${NC} \e]8;;file://$(pwd)/$file_path\e\\Click Here\e]8;;\e\\"
        done

    fi
fi

# Function to show motivational message based on progress
show_motivational_message() {
    local curr_errors="$1"
    local curr_passes="$2"
    local curr_failures="$3"
    local curr_compile_errors="$4"
    local curr_runtime_errors="$5"
    local curr_efficiency_ratio="$6"
    local curr_pure_test_time="$7"
    local curr_challenge="$8"
    local prev_errors="$9"
    local prev_passes="${10}"
    local prev_failures="${11}"
    local prev_compile_errors="${12}"
    local prev_runtime_errors="${13}"
    local prev_efficiency_ratio="${14}"
    local prev_pure_test_time="${15}"
    local prev_improvement_streak="${16}"
    local prev_current_challenge="${17}"
    local prev_timestamp="${18}"
    # Calculate changes with detailed context - avoid double-counting test failures
    # curr_errors represents files with test failures, curr_failures represents individual test failures
    # We should only count one or the other, not both, to avoid double-counting the same issue
    local total_curr_issues=$((curr_failures + curr_compile_errors + curr_runtime_errors))
    local total_prev_issues=$((prev_failures + prev_compile_errors + prev_runtime_errors))
    local total_change=$((total_curr_issues - total_prev_issues))
    local pass_change=$((curr_passes - prev_passes))
    local efficiency_change=$((curr_efficiency_ratio - prev_efficiency_ratio))
    local compile_change=$((prev_compile_errors - curr_compile_errors))
    local runtime_change=$((prev_runtime_errors - curr_runtime_errors))

    # Calculate improvement streak
    local current_streak=$prev_improvement_streak
    if [ "$total_change" -lt 0 ]; then
        current_streak=$((current_streak + 1))
    elif [ "$total_change" -gt 0 ]; then
        current_streak=0
    fi

    # Expanded mythological greetings with historical events and seasonal context
    local hour=$(date +%H)
    local month=$(date +%m)
    local day=$(date +%d)
    local greeting=""
    local greeting_context=""

    # Create arrays of greetings for different times and contexts
    declare -a dawn_greetings=(
        # --- Character Building: Dawn Reflections ---
        "🌅  DAEDALUS'S RITUAL|Before touching the blueprints, Daedalus stares east towards Athens, his expression a mixture of profound longing and bitter resentment for the city that exiled him."
        "🪨  THE SAME FIRST STEP|The rising sun illuminates the same chalk mark on the floor where yesterday's work began. You realize with a chill, Sisyphus, that every dawn begins in the exact same place."
        "👑  MINOS AT DAWN|The King is already watching from his balcony. He is not inspecting your work; he is staring at the Labyrinth's dark, silent entrance, as if expecting something to emerge."

        # --- Dialogue: Words at Sunrise ---
        "🗣️  DAEDALUS'S FIRST COMMAND|'Forget yesterday's victories, Sisyphus. The sun has risen, and it has revealed our failures. The King only cares about the progress we make today.'"
        "🗣️  A GUARD'S GRIM JOKE|The new guard bringing your breakfast mutters, 'Fresh bread for the builders, fresh meat for the beast. We all serve the Labyrinth's appetite, eh?'"
        "🗣️  THE ECHO OF A NIGHTMARE|Daedalus, pale and shaken, grabs your arm. 'I dreamt of my nephew, Talos. He was laughing and pointing at a single flaw in the foundation. Find it, Sisyphus. Find it now.'"

        # --- Storytelling: The Labyrinth at First Light ---
        "🌅  A CRUEL DAWN|The first rays of sun strike the Labyrinth walls, revealing a subtle flaw in the code you wrote by candlelight. The light of truth is a harsh inspector."
        "🌅  THE PALACE AWAKENS|The sounds of the waking palace echo from below. The secret, frantic work of the night must now become the deliberate, measured labor of the day."
        "📜  THE MORNING DECREE|A messenger arrives with the dawn, carrying a wax tablet. 'King Minos has a new requirement. A section of the maze must be redesigned by dusk.' The work is never done."

        # --- Character Building: Dawn Reflections ---
        "☕️  DAEDALUS'S FUEL|The architect drinks his morning brew, staring intently at the code. 'The night gives us mystery, Sisyphus, but the dawn gives us clarity. And clarity reveals mistakes.'"
        "🪨  NO RESPITE|As the sun crests the horizon, you realize the hope of a new day is an illusion. For you, Sisyphus, it is only the same boulder, now in a different light."
        "🩸  THE BEAST MUST FEED|A fleet of guards lead a tribute towards the Labyrinth's entrance as the sun rises. A stark reminder of the grim purpose of your perfect prison."

        # --- Dialogue: Words at Sunrise ---
        "🗣️  A HARSH AWAKENING|Daedalus points a trembling finger at a complex function. 'Look! What seemed like genius in the dark is madness in the light of day. Fix it. Now.'"
        "🗣️  THE CHANGING OF THE GUARD|A new sentry, seeing you already at work, mutters, 'You two never rest. Does the King have you building his tomb, or just hiding his secrets?'"
        "🗣️  A GRIM INVENTORY|Daedalus points to a pile of papyrus scrolls. 'Each of these is a flaw we thought was fixed yesterday. The dawn has shown us our folly. We begin again, Sisyphus.'"

        # --- The Nature of the Curse ---
        "🤔 A TASK WITHOUT MEANING|The sun rises, and with it, the gnawing question: Does perfecting this code truly matter? The boulder, at least, had the simple honesty of being pointless."
        "✨ THE ILLUSION OF CHOICE|You pick up a different tool this morning, start on a different function. A small act of rebellion. But by midday, you know all paths lead back to the same endless task."
        "🔥 A FATE WORSE THAN THE STONE|A horrifying thought strikes you as you begin to code: The gods did not simply change your punishment. They refined it. A boulder punishes the body, but a perfect, endless puzzle... that punishes the mind."

        # --- Whispers of the Palace ---
        "😡 THE KING'S MORNING RAGE|A muffled roar echoes from the royal chambers. A servant whispers that King Minos had another nightmare about his 'son' and has already executed a guard for looking at him wrong."
        "🥛 THE NURSEMAID'S PRAYER|You see the beast's old nursemaid leaving a small offering of milk and honey at a sealed-off entrance to the maze. She prays not to the gods, but to the stones themselves."
        "🤫 A SPY'S SHADOW|You catch a fleeting glimpse of a figure on a distant rooftop, watching your workshop through a polished bronze mirror. Daedalus was right. Someone is watching."

        # --- The Labyrinth's Awakening ---
        "🧠 A PATTERN IN THE BUGS|The errors from last night's code have... rearranged themselves. They form a new, more elegant problem. It feels less like a series of mistakes and more like an intelligent response."
        "🌀 THE MAZE'S FIRST BREATH|As the morning air warms the stones, a low, resonant hum emanates from the foundations. It is the sound of a vast machine stirring to life for the first time."
        "🏞️ A CORRIDOR OF MEMORY|You walk down a hall you finished yesterday, and for a moment, the cold stone walls seem to flicker with an image of a rolling green hill. The Labyrinth is showing you what you've lost."
    )

    declare -a morning_greetings=(
        # --- Character Building: Mid-Morning Tensions ---
        "📜  A SECRET KEY|You notice Daedalus using a separate, smaller set of scrolls for his most complex calculations, hiding them whenever a guard comes near. These are not part of the official plans."
        "⛓️  THE WEIGHT OF THE CURSE|You see a group of condemned tributes being marched from the docks toward the Labyrinth. Their vacant eyes meet yours, and you feel the true weight of the prison you are building."
        "🐂  THE BEAST'S ANGER|A shipment of massive, bronze-enforced stones arrives. Daedalus explains, 'The Minotaur broke through the inner wall again last night. My original designs were not strong enough.'"

        # --- Dialogue: Tense Morning Exchanges ---
        "🗣️  DAEDALUS'S PERFECTIONISM|'The bronze content in these hinges is insufficient,' Daedalus barks, throwing a component across the workshop. 'Tell the forge to triple the mix. The King's beast will not be contained by shoddy work.'"
        "🗣️  THE SILENT WATCHERS|The morning shift of guards is new. They do not speak. They only stand at the corners of the workshop, their eyes fixed on your hands as you code."
        "🗣️  A BIZARRE MODIFICATION|A new decree from Minos arrives. 'No right angles in the western wing,' Daedalus reads from a wax tablet, bewildered. 'The King says corners give false hope. He wants only curves.'"

        # --- Storytelling: The Labyrinth in the Full Light of Day ---
        "☀️ THE SUN'S HARSH GLARE|With the sun high in the sky, you and Daedalus can no longer hide your work. The sounds of your tools echo, and you feel the gaze of curious eyes from the palace walls."
        "🔍  THE ROYAL INSPECTION|A royally-appointed surveyor arrives unannounced, demanding to see the progress. 'King Minos wants assurances this structure is as inescapable as promised,' he sneers."
        "❓  A LOGICAL PARADOX|A corridor you perfected yesterday has inexplicably changed overnight. Daedalus suspects sabotage; you suspect the Labyrinth itself is actively resisting its own creation."

        # --- Character Building: Mid-Morning Tensions ---
        "🧠  SISYPHUS'S MENTAL FATIGUE|The physical weight of the boulder is gone, replaced by the crushing mental weight of endless logic puzzles. Each bug feels like the boulder slipping from your grasp, threatening to roll all the way back to the bottom."
        "🔬  DAEDALUS'S PARANOIA|The architect grows paranoid, his eyes darting at every shadow. He examines your code, whispering about Athenian spies sent to steal his Labyrinth's secrets."
        "👑  THE QUEEN'S SORROW|Queen Pasiphaë walks the palace gardens, always within sight of the tower where you work. Her gaze is not one of pride, but of profound, unending grief."

        # --- Dialogue: Tense Morning Exchanges ---
        "🗣️  DAEDALUS'S FRUSTRATION|'This function should be perfect! Did you follow the blueprint exactly, Sisyphus? Or has some god cursed this stone and code to defy my will?'"
        "🗣️  GOSSIPING GUARDS|Two sentries whisper as you pass. 'The architect is mad, they say. And his assistant... that one has the haunted look of a man who has seen Tartarus and returned.'"
        "🗣️  DAEDALUS'S GRIM REQUEST|The architect looks over his plans. 'We need more lead for the inner chamber doors, Sisyphus. Not for strength... to muffle the sounds. For the Queen's sake.'"

        # --- The King's Scrutiny ---
        "🗺️  THE KING'S MEASURE|A royal surveyor arrives with a perfectly balanced scale. 'King Minos demands we weigh every stone,' he says. 'He believes a single ounce of imperfection will compromise the entire structure.'"
        "🐦  THE SILENT BIRDS|Every morning, crows gather on the workshop roof. Today, they're completely silent, all facing the same direction - toward a tower that wasn't there yesterday. Daedalus pretends not to notice, but his hands shake as he draws."
        "🤫 A VOW OF SILENCE|A new royal decree is posted. The punishment for speaking of the 'architect's work' outside the palace walls is now death. The King is burying his secret in silence and terror."

        # --- The Labyrinth's Nature ---
        "🌀 IMPOSSIBLE GEOMETRY|You spend the morning building a perfectly straight corridor, only to return after lunch and find a slight, almost imperceptible curve in it. The stones have settled... or shifted."
        "💧 THE DRAIN OF VITALITY|The plants in the workshop have withered overnight, despite being watered. The air feels thin. It's as if the Labyrinth is drawing the life out of everything around it."
        "🗺️ THE FIRST MAZE|In a dusty crate, you find an older, smaller blueprint for a different maze. It's crossed out with a single word written in dried blood: 'INSUFFICIENT'."

        # --- The Weight of the Curse ---
        "🧠 THE LOGIC TRAP|You realize the most difficult bugs aren't errors in logic, but loops that are perfectly logical, yet never-ending. The code works exactly as designed... and that is the problem."
        "🎭 A COMEDY OF THE GODS|You fix a bug, and Daedalus immediately finds another. You see the humor in it—a dark, divine comedy where you are the punchline. You almost laugh."
        "⌛️ THE NATURE OF 'DONE'|You mark a module as 'complete.' The word feels hollow, meaningless. In this place, 'done' is just the quiet before the next bug report, the brief peace before the boulder rolls again."
    )

    declare -a afternoon_greetings=(
        # --- Character Building: Afternoon Despair ---
        "⏳  THE ILLUSION OF PROGRESS|Daedalus holds two identical-looking blueprints. 'I have fixed this bug seven times, Sisyphus. Every time, it returns in a new form. This maze... it heals itself.'"
        "⛓️  A PRISONER'S GAZE|You find Daedalus tracing the patterns of the iron bars on the workshop window. 'The perfect trap, Sisyphus,' he says quietly, 'is the one the prisoner helps you build.'"
        "🔥  THE HEAT OF THE FORGE|The afternoon heat reminds you of Tartarus. You wonder if this is a crueler hell: to be aware, to have a purpose, but to know that purpose is to build a monument to someone else's shame."

        # --- Dialogue: Heated Afternoon Exchanges ---
        "🗣️  THE SURVEYOR'S TAUNT|The royal inspector returns. 'Still at it? I wonder, architect, which is the greater marvel: your Labyrinth, or the King's patience with your endless delays?'"
        "🗣️  A DESPERATE BARGAIN|Daedalus whispers to you, his eyes wide. 'If you help me with my... personal project... I can create a flaw in the maze only we know. A way out, for both of us.'"
        "🗣️  THE MINOTAUR'S LAMENT|Amidst the pounding of hammers, you hear a new sound from below: not a roar of rage, but a long, sorrowful bellow. It is a sound of profound loneliness."

        # --- Storytelling: The Labyrinth Under the Oppressive Sun ---
        "🔎  AN IMPOSSIBLE DISCOVERY|Daedalus holds up a blueprint, his hands shaking. 'This sequence is not mine. Someone has sabotaged the core logic!' The paranoia of the morning now feels terrifyingly real."
        "😱  A SCREAM FROM BELOW|A piercing scream echoes from the Labyrinth's lower levels, abruptly silenced. The palace guards look at each other, then quickly look away.|Your 'bug-fixes' and 'features' are not abstract concepts. They are the stone and mortar of a tomb. The work continues."
        "🧠  THE ARCHITECT'S MADNESS|You find Daedalus carving equations directly onto the stone walls of his chamber. 'The papyrus lies!' he screams. 'The truth is in the stone! Only the stone!'"

        # --- Character Building: Afternoon Despair ---
        "⛓️  SISYPHUS'S REALIZATION|You realize this is worse than the boulder. The boulder was honest in its absurdity. This Labyrinth pretends to have a solution, a final perfect state, but you suspect it's a lie."
        "✍️  DAEDALUS'S FRANTIC SKETCHES|The architect, slick with sweat, abandons the main blueprints and furiously sketches on a scrap of papyrus. It’s a wing, made of feathers and wax. A desperate, mad idea."
        "🐂  THE LABYRINTH TREMBLES|A low, rhythmic *thud* echoes from deep within the maze, shaking the dust from the stones. The Minotaur is not resting. It is testing your work, Sisyphus, looking for a flaw."

        # --- Dialogue: Heated Afternoon Exchanges ---
        "🗣️ MINOS'S THREAT|The King, walking the grounds below, shouts up at the tower. 'The tribute from Athens arrives in three days, Daedalus! If this maze is not ready to receive them, your son will be the first to test its corridors!'"
        "🗣️ A SKEPTICAL GUARD|A passing sentry looks at your complex work and scoffs. 'All this stone and genius to hold one boy? I think the King's true monster is his own pride.'"
        "🗣️ A CHILLING REALIZATION|Daedalus stops you before you enter a newly completed section. 'Wait. I designed this corridor to be inescapable from the inside. Let us pray, Sisyphus, that the King never asks us to walk it.'"

        # --- The Labyrinth's Malevolence ---
        "🩸 THE WALLS BLEED|The oppressive heat makes the newly-laid mortar sweat a strange, red dust, like the stone itself is weeping blood. The Labyrinth feels sickly, alive."
        "🌀 A SHIFTING PERSPECTIVE|You look at a section of code you wrote this morning and no longer recognize it. The logic seems alien, hostile. It's as if the Labyrinth is rewriting itself behind you."
        "👂 UNHEARD MELODIES|Daedalus puts his ear to a wall. 'Do you hear that? A song... but the notes are all wrong. The harmony is a weapon.' You hear nothing but the wind."

        # --- The Weight of Secrets ---
        "🔑 THE HIDDEN KEY|While organizing scrolls, you find a complex diagram for a key, one unlike any you've seen. Daedalus snatches it from your hands. 'That is for a door that does not yet exist,' he hisses."
        "📜 THE SECOND LEDGER|You discover Daedalus keeps two sets of progress reports. One, filled with triumphs, is for the King. The other, filled with cryptic warnings and sketches of wings, is for himself."
        "👥 THE SHADOW COUNCIL|You see Daedalus meeting in a secluded courtyard with two figures you don't recognize. They are not Cretan. They speak in a rapid, hushed Athenian dialect."

        # --- Echoes of the Past ---
        "🐦 THE PARTRIDGE'S CURSE|A single partridge lands on the windowsill. Daedalus freezes, his face contorted in a mask of pure terror, until the bird flies away. He does not speak for an hour."
        "💬 A GUARD'S QUESTION|A curious sentry asks you what it was like in Tartarus. You look at the endless, complex code before you and reply, 'This is worse. In Tartarus, at least I knew what the punishment was.'"
        "🔥 THE FLAMES OF ENVY|'I was celebrated in Athens,' Daedalus mutters to the forge fire. 'Now I am a zookeeper for a king's shame. A fine legacy, built from the ashes of my own jealousy.'"
    )

    declare -a evening_greetings=(
        # --- Character Building: Evening Thoughts ---
        "🗺️ THE LABYRINTH'S HEART|Daedalus reveals the center of the blueprint. It is not a room, but a complex, shifting mechanism. 'It is designed to constantly reconfigure,' he says, 'so that even I will not know the true path.'"
        "🦉 SISYPHUS'S VOW|Watching the sunset, you make a silent promise. You may be cursed to this task, but you will learn every secret of this code. You will know this Labyrinth better than its own creator."
        "🧠 A SENTIENT DESIGN|Daedalus stares at the completed western wing blueprint. 'It is no longer just a design, Sisyphus,' he whispers. 'The logic has become... aware. It learns from those who enter. It *hunts*.'"

        # --- Dialogue: Voices in the Gloom ---
        "🗣️  THE GHOST IN THE MACHINE|'I know there's an error in the central routing logic,' Daedalus whispers, holding a candle to a blueprint. 'I can *feel* it, like a cold draft from a crack in the stone, but the plans say it's perfect.'"
        "🗣️  A GUARD'S PHILOSOPHY|An old sentry, watching you work late, says quietly, 'I've seen many men build their own prisons. You are the first I've seen who seems to understand the shape of it.'"
        "🗣️  THE EVOLVING BEAST|'The sounds from the maze are changing,' Daedalus murmurs, listening intently. 'The rage is less frequent now. I hear... a lament. A beast that is beginning to understand its own purpose.'"

        # --- Storytelling: The Labyrinth at Dusk ---
        "🌆 LABYRINTH AT TWILIGHT|As the sun sets over Crete, the Labyrinth's shadows lengthen, twisting the corridors into new, more confusing shapes. The work must continue by torchlight."
        "🔥 THE NIGHT'S WORK BEGINS|Daedalus unrolls fresh blueprints under the evening stars. The most intricate and secret parts of the maze are designed while the rest of the kingdom sleeps."
        "🌙 A REPORT FOR THE KING|A palace guard arrives, his torch casting long shadows. 'King Minos demands a report on the day's progress. He grows impatient to see his masterpiece complete.'"

        # --- Character Building: Evening Thoughts ---
        "🦉 DAEDALUS'S OBSESSION|The master architect doesn't sleep. He stalks the corridors at night, muttering about angles and dead-ends, his mind consumed by the perfect prison."
        "🐂 THE MINOTAUR WAKES|As darkness falls, a low growl echoes from deep within the half-finished maze. The beast knows you are building its cage, and it despises the encroaching stone."
        "🪨 A NEW KIND OF BURDEN|Sisyphus, as you rest your weary bones, you realize this is a different curse. The boulder was mindless; the code is a puzzle that follows you into your dreams."

        # --- Dialogue: Voices in the Gloom ---
        "🗣️  DAEDALUS'S WHISPER|'Bring the lanterns, Sisyphus. The day is for labor, but the night is for genius. Let us solve this puzzle before the sun returns.'"
        "🗣️  A CRETAN'S WARNING|An old guard whispers to you, 'Be wary. These walls hear everything at night. Even the King's secrets are not safe within this maze.'"
        "🗣️  THE HERO'S VOW|A faint echo on the sea breeze sounds like a young man's voice... 'Ariadne, I swear on this evening star, I will conquer the maze and end this terror.'"

        # --- The Labyrinth's Deepening Mystery ---
        "🧩 THE PUZZLE SOLVER|'It's learning,' Daedalus says, his voice hollow. 'I laid a false path yesterday as a test. Today, the stones are rearranged. It's not just a prisoner, Sisyphus. It's a rival architect.'"
        "🎶 A HAUNTING MELODY|Daedalus stops working, his head cocked. 'That sound... it's not a growl. It's a song. The Queen used to sing it to him.' The faint, distorted melody is deeply unsettling."
        "🔒 THE FORBIDDEN DOOR|Daedalus points to a section of the plan. 'This door must never be opened once sealed. Not by you, not by me, not even by Minos himself. Swear it, Sisyphus.'"

        # --- The Weight of the Curse ---
        "💰 A SILENT PAYMENT|A palace official leaves a heavy purse of gold on the table without a word. The payment feels less like a salary and more like a bribe to ensure your silence."
        "📜 THE UNSEEN CLAUSE|You notice fine print on a blueprint you've never seen before. It's a contract, binding Daedalus to the King's service until 'the project and all its secrets are secure.'"
        "🌀 THE CYCLE REVEALED|You fix a bug in a function, and watch in dawning horror as an identical, but subtly different, bug appears in a library you completed weeks ago. The Labyrinth does not want to be finished."

        # --- Echoes of the Outside World ---
        "🚢 A SHIP IN THE NIGHT|A single ship with black sails slips into the harbor under the cover of darkness. It carries no cargo, only passengers who do not disembark in the main port. The tribute system has begun."
        "🎭 THE FORGOTTEN STAGE|'I once designed moving stages for the theaters in Athens,' Daedalus says wistfully, looking at a complex gear mechanism. 'Now I build only cages. A fine use of my talents.'"
        "🦉 ATHENA'S SILENCE|'She has abandoned me,' Daedalus mutters, looking up at the sky. 'Athena, my patron, my guide... she has been silent ever since I arrived on this cursed island. There is no wisdom here. Only madness.'"
    )

    declare -a late_night_greetings=(
        # --- Character Building: Late Night Reflections ---
        "🤫  THE HIDDEN PASSAGE|While reviewing the day's code, you spot it: a single, commented-out function. A backdoor. You look over at Daedalus, but he is sleeping. You wonder if he left it intentionally, or if he's forgotten it's there."
        "❓  AN IMPOSSIBLE FLANK|You trace a path for the Minotaur to its chamber, but realize the logic creates a perfect pincer, trapping not just the beast, but anyone who pursues it. You wonder if the Labyrinth's true prey is the hero, not the monster."
        "🩸  THE BLOOD MOON|A lunar eclipse casts a red glow over the workshop. Daedalus wakes with a start, screaming a name you don't recognize: 'Perdix!' He stares at his hands in horror before collapsing back into sleep."

        # --- Dialogue: Whispers After Midnight ---
        "🗣️  'IT HAS NO CENTER'|Daedalus, speaking in his sleep, murmurs, 'The secret... Sisyphus... is that the maze has no center. The Minotaur is just... the first prisoner. The true monster is the Labyrinth itself.'"
        "🗣️  A VOICE FROM THE WALLS|'Listen...' Daedalus whispers, his eyes wide with terror. 'The walls... I hear whispers from the stone. It's speaking in an Athenian dialect. It knows where I came from.'"
        "🗣️  KING MINOS IN THE DARK|You look down and see a single figure walking the perimeter of the Labyrinth in the dead of night. It is King Minos, alone, inspecting the prison that holds his greatest shame and his greatest weapon."

        # --- Storytelling: The Labyrinth in the Deep Night ---
        "🤫 SECRETS IN THE STONE|In the dead of night, the Labyrinth seems to breathe. The stones shift, revealing passages that Daedalus himself doesn't remember designing. The maze has a will of its own."
        "🗺️  A FORGOTTEN BLUEPRINT|You find a hidden scroll under a loose floor tile. It's an older, more chaotic design for the Labyrinth, signed by Daedalus with a trembling hand and a note: 'Too dangerous.'"
        "🕯️  THE LAST CANDLE BURNS|The entire palace sleeps, but you and Daedalus work on. The final candle flickers, its small light a lonely beacon against the immense, dark puzzle of the code."

        # --- Character Building: Late Night Reflections ---
        "🤔  SISYPHUS'S REALIZATION|You stare at a complex function and realize the true curse isn't the boulder, but the tantalizing proximity of a perfect, elegant solution that is always just one step away."
        "👑  MINOS'S NIGHTMARES|Even the King of Crete cannot rest. He is haunted by dreams of his son, the Minotaur, and the fear that even Daedalus's genius cannot contain his rage forever."
        "🐂  THE BEAST IN THE DARK|In the profound silence of midnight, you can almost hear the Minotaur's slow, heavy breathing. It is not sleeping. It is waiting. The code must be perfect to hold it."

        # --- Dialogue: Whispers After Midnight ---
        "🗣️  DAEDALUS'S CONFESSION|The architect, weary and half-asleep, looks at you. 'I fear what I have built, Sisyphus. I fear this logic we've created is a trap for more than just the monster.'"
        "🗣️  A PALACE SERVANT'S WHISPER|A servant, fetching water in the dead of night, whispers to you, 'I pity the creature. They say it's the Queen's own son. King Minos builds this maze to hide his own shame.'"
        "🗣️  DAEDALUS'S LATE NIGHT DOUBT|The architect stares into the flickering lamplight. 'I worry, Sisyphus. What if the Labyrinth is too perfect? What if we build a prison that even its own creator cannot escape?'"

        # --- The Labyrinth's True Nature ---
        "🧠 THE LIVING ARITHMETIC|Daedalus traces a line of code. 'This is not architecture, Sisyphus. This is a calculation that thinks. We are not building walls; we are teaching a god how to reason.'"
        "🌀 THE ENDLESS CORRIDOR|You find a logical loop that, by all accounts, should be infinite. Yet, it terminates. When you ask Daedalus, he simply says, 'The Labyrinth permits it. Do not question its nature.'"
        "🎯 A PRISON FOR THOUGHTS|'Do you see now?' Daedalus whispers, pointing to the dizzying array of functions. 'This maze will not contain a beast of flesh and bone. It is designed to trap a mind. Any mind.'"

        # --- The Architect's Guilt ---
        "👻 A RIVAL'S GHOST|'He was better than me,' Daedalus mutters, staring at an elegant function you wrote. 'So much promise. Such a waste.' He is not talking about your code."
        "🗼 THE VIEW FROM THE ACROPOLIS|Daedalus stands by the high window, looking down. 'From a great height,' he says, his voice hollow, 'everything looks small enough to be discarded. A dangerous perspective.'"
        "👐 THE IDLE HANDS OF A MURDERER|You see Daedalus holding his hands over the candlelight, examining them. 'These hands have created wonders,' he whispers, 'and committed horrors. I wonder which the gods will remember.'"

        # --- The Prisoner's Perspective ---
        "🪨 THE ECHOING BOULDER|In the dead silence, you can almost hear the scrape of a great boulder on a stone path. You realize with horror that the sound is only in your head, a memory of a simpler torment."
        "🤝 A SHARED DAMNATION|You press your ear to the floor and hear a faint, rhythmic tapping from below. One-two. One-two. The Minotaur is not raging. It is counting the seconds, just as you are."
        "🎭 PERFECTION FOR NO ONE|A chilling thought: you are building a perfect prison that no one will ever see, to contain a creature whose only crime was being born. Your eternal labor serves only to hide a King's shame."
    )

    # --- Seasonal Narrative Greetings ---
    # These messages are rare and designed to have a significant impact on the story,
    # revealing the core themes of the eternal, cyclical curse.

    declare -a spring_greetings=(
        # --- Theme: A Cruel Renewal ---
        "🌸 A MOCKERY OF SPRING|Flowers bloom outside the tower walls, a bitter jest from the gods. For you, Sisyphus, there is no renewal, only the same code, the same stones, the same eternal beginning."
        "🌱 THE FIRST TRIBUTE|Spring brings the first black-sailed ships from Athens. The tributes, meant to test the maze, arrive with the season of sacrifice. Daedalus bars the workshop door."
        "🦋 A FLUTTER OF FALSE HOPE|A butterfly lands on the windowsill. Daedalus watches it, his face a mask of envy, before turning back to the blueprints. 'There is no escape for us,' he mutters."
        "💧 DEMETER'S BARGAIN|The earth is reborn, but Queen Pasiphaë's sorrow does not lift. She made a bargain for her monstrous son's life, and the price is this perfect, inescapable prison you build."
        "🤔 A FOOL'S HOPE|You watch the new growth outside and a foolish thought enters your mind: 'Perhaps this time will be different. Perhaps this time, the code will hold.' You know it is a lie, but you believe it for a moment."
        "🗣️ DAEDALUS'S DECREE|'The spring rains have weakened the foundation,' Daedalus declares, pointing to a fresh crack. 'Everything we built last season is now suspect. We must verify it all again, from the beginning.'"
    )

    declare -a summer_greetings=(
        # --- Theme: Oppressive Heat, Inescapable Fate ---
        "🔥 THE FORGE OF HELL|The summer sun turns the workshop into an oven. Daedalus claims the heat reminds him of Hephaestus's forge, but his wild eyes tell you it reminds him more of the fires of Tartarus."
        "🔥 MINOS'S BURNING GAZE|The King spends the long, hot days watching your progress from his shaded balcony. His impatience grows with the heat, his demands becoming more erratic and impossible."
        "🐍 A PLAGUE OF ERRORS|The oppressive heat seems to spawn bugs like insects in a swamp. Every line of code you fix, two more seem to appear in its place, as if the Labyrinth itself is feverish."
        "🌊 THE SALT-STAINED BLUEPRINTS|The sea air is thick and heavy. The papyrus scrolls curl with dampness, blurring the lines of the master plan. Daedalus accuses the sea god himself of trying to sabotage his work."
        "🤔 A FAMILIAR BURN|The heat of the forge where the Labyrinth's iron is shaped feels familiar. You recall the flames of the underworld and wonder which punishment is worse: a simple, honest fire, or this sun-baked intellectual torment."
        "🗣️ A GUARD'S CYNICISM|A guard, bringing you water, mutters, 'The King hides from the sun while we toil in it. He fears the light will reveal the true monster is not the one in the maze.'"
    )

    declare -a autumn_greetings=(
        # --- Theme: Decay and the Inevitable Harvest ---
        "🍂 THE HARVEST OF TRIBUTES|As the world outside gathers its crops, Crete prepares for its own grim harvest. The Labyrinth's gates are oiled, the tribute chambers are cleaned. The machine awaits its purpose."
        "🌬️ A CHILLING WIND|The first autumn wind howls through the tower, scattering scrolls. For a moment, it sounds like a scream from the maze below. Daedalus doesn't even flinch anymore."
        "🥀 THE QUEEN'S GARDEN WILTS|Queen Pasiphaë no longer walks the gardens. The flowers have died, and she remains in her chambers. Her sorrow has consumed the season, a blight upon the land."
        "🦉 THE OWL'S JUDGMENT|An owl, Athena's sacred bird, lands on the windowsill each evening. Daedalus, once her favored craftsman, refuses to look at it, as if terrified of the wisdom—or condemnation—it might offer."
        "🤔 A HARVEST OF NOTHING|You watch the farmers bringing in their bounty. You have labored just as they have, yet your only harvest is a more perfect prison, a more complete damnation. There is no crop to gather from your work."
        "🗣️ THE HERO'S ARRIVAL|'A ship from Athens,' the guard captain reports to Daedalus. 'It carries a volunteer. A prince named Theseus. He says he has come to slay the beast.' Daedalus only smiles, a thin, cruel line on his lips."
    )

    declare -a winter_greetings=(
        # --- Theme: Isolation, Cold Clarity, and Existential Dread ---
        "❄️ THE SILENCE OF THE TOMB|The first snow blankets the palace in silence. The Labyrinth below feels less like a prison and more like a mausoleum. You are not building a cage; you are perfecting a tomb for a living god."
        "🥶 THE POINT OF NO RETURN|Trapped by winter storms, Daedalus stares at the master blueprint for days without speaking. At last, he whispers, 'Sisyphus... I have designed it so perfectly that I no longer know the way out myself.'"
        "🕯️ THE ETERNAL NIGHT|The nights are longest now. You have more hours in the dark with the code than in the light. You feel more at home in the logical maze of the blueprints than in the waking world."
        "🧊 A FROZEN HEART|King Minos's demands cease. He no longer watches from his balcony. In the cold of winter, even his rage seems to have frozen, replaced by a cold, calculating acceptance of the monster he keeps."
        "🤔 A COLD COMFORT|The biting winter wind feels like a relief. It is a clean, honest pain, unlike the twisted, complex agony of a recursive function with no exit condition."
        "🗣️ DAEDALUS'S CHILLING PROPHECY|'The Labyrinth will be finished when the winter ends, Sisyphus,' Daedalus says, his breath misting in the cold air. 'And when it is... the King has no more use for its architect. Or his assistant.'"
    )

    # Select greeting based on time and season
    if [ "$hour" -lt 6 ]; then
        selected_greeting=$(echo "${late_night_greetings[RANDOM % ${#late_night_greetings[@]}]}")
    elif [ "$hour" -lt 9 ]; then
        selected_greeting=$(echo "${dawn_greetings[RANDOM % ${#dawn_greetings[@]}]}")
    elif [ "$hour" -lt 12 ]; then
        selected_greeting=$(echo "${morning_greetings[RANDOM % ${#morning_greetings[@]}]}")
    elif [ "$hour" -lt 18 ]; then
        selected_greeting=$(echo "${afternoon_greetings[RANDOM % ${#afternoon_greetings[@]}]}")
    else
        selected_greeting=$(echo "${evening_greetings[RANDOM % ${#evening_greetings[@]}]}")
    fi

    # Add seasonal flavor (20% chance to override with seasonal greeting)
    if [ $((RANDOM % 5)) -eq 0 ]; then
        if [ "$month" -ge 3 ] && [ "$month" -le 5 ]; then
            selected_greeting=$(echo "${spring_greetings[RANDOM % ${#spring_greetings[@]}]}")
        elif [ "$month" -ge 6 ] && [ "$month" -le 8 ]; then
            selected_greeting=$(echo "${summer_greetings[RANDOM % ${#summer_greetings[@]}]}")
        elif [ "$month" -ge 9 ] && [ "$month" -le 11 ]; then
            selected_greeting=$(echo "${autumn_greetings[RANDOM % ${#autumn_greetings[@]}]}")
        else
            selected_greeting=$(echo "${winter_greetings[RANDOM % ${#winter_greetings[@]}]}")
        fi
    fi

    # Add special historical date greetings (override everything else)
    if [ "$month" -eq 3 ] && [ "$day" -eq 15 ]; then
        selected_greeting="🗡️ IDES OF MARCH|'Beware the Ides!' - Caesar's fateful day teaches vigilance"
    elif [ "$month" -eq 4 ] && [ "$day" -eq 6 ]; then
        selected_greeting="🏃 MARATHON COMMEMORATION|Pheidippides' legendary run inspires endurance"
    elif [ "$month" -eq 8 ] && [ "$day" -eq 1 ]; then
        selected_greeting="🏛️ ATHENIAN DEMOCRACY|birth of democratic ideals shapes the world"
    elif [ "$month" -eq 9 ] && [ "$day" -eq 2 ]; then
        selected_greeting="✨ BATTLE OF ACTIUM|Octavian's victory reshapes the ancient world"
    elif [ "$month" -eq 10 ] && [ "$day" -eq 19 ]; then
        selected_greeting="🍇 OSCHOPHORIA FESTIVAL|Athenian harvest celebration honors Dionysus"
    elif [ "$month" -eq 11 ] && [ "$day" -eq 8 ]; then
        selected_greeting="🏛️ SOCRATES TRIAL|philosopher faces death for pursuing wisdom"
    elif [ "$month" -eq 12 ] && [ "$day" -eq 25 ]; then
        selected_greeting="🌟 WINTER SOLSTICE|ancient Greeks honored the return of light"
    fi

    # Parse the selected greeting
    IFS='|' read -r greeting greeting_context <<< "$selected_greeting"

    # Recalculate progress bar variables for THE FINAL DOPAMINE HIT!
    local legend_line="$(read_legend_stats)"
    IFS='|' read -r total_runs total_quests hydra_beasts blueprint_flaws oracle_mysteries divine_streaks longest_streak perfect_ascensions first_quest total_project_xp project_level <<< "$legend_line"

    # Calculate XP progress within current level using C header formula
    get_xp_for_level() {
        local level=$1
        echo $(( 1000 + (level * level * level * 200) + (level * level * 600) + (level * 300) ))
    }

    get_current_level_from_xp() {
        local total_xp=$1
        local level=1
        local cumulative_xp=0

        while true; do
            local xp_needed=$(get_xp_for_level $level)
            if [ $((cumulative_xp + xp_needed)) -gt $total_xp ]; then
                break
            fi
            cumulative_xp=$((cumulative_xp + xp_needed))
            level=$((level + 1))
        done
        echo $level
    }

    get_xp_in_current_level() {
        local total_xp=$1
        local current_level=$2
        local cumulative_xp=0

        for ((i=1; i<current_level; i++)); do
            cumulative_xp=$((cumulative_xp + $(get_xp_for_level $i)))
        done
        echo $((total_xp - cumulative_xp))
    }

    # Recalculate project level based on total XP using correct formula
    local actual_project_level=$(get_current_level_from_xp $total_project_xp)
    local current_level_xp=$(get_xp_in_current_level $total_project_xp $actual_project_level)
    local xp_needed_for_next=$(get_xp_for_level $actual_project_level)

    # Draw progress bar as separator before mythological messages - THE FINAL DOPAMINE HIT!
    local width=20
    local filled=$(( current_level_xp * width / (xp_needed_for_next > 0 ? xp_needed_for_next : 1) ))
    echo ""
    echo -n -e "${CYAN}🎯 ${BOLD_WHITE}$(format_number_with_commas $total_project_xp)${NC} ${CYAN}Project XP [${NC}"
    for ((i=0; i<width; i++)); do
        if [ $i -lt $filled ]; then
            echo -n "█"
        else
            echo -n "░"
        fi
    done
    echo -e "${CYAN}] $(format_number_with_commas $current_level_xp)/$(format_number_with_commas $xp_needed_for_next) ${BOLD_WHITE}(Level $actual_project_level)${NC}"
    # Apply bulk mode XP reduction (1/4 XP in bulk mode) BEFORE penalties
    local display_tests_xp=$SESSION_XP_FROM_TESTS
    local display_combos_xp=$SESSION_XP_FROM_COMBOS
    local display_achievements_xp=$SESSION_XP_FROM_ACHIEVEMENTS
    local display_bulk_penalty=$BULK_FAILURE_PENALTY
    local display_original_xp=$original_session_xp

    if [ "$SISYPHUS_FULL_TEST_MODE" = "1" ] && [ "$SUCCESSFULLY_RUN_FILES" -gt 0 ]; then
        display_tests_xp=$((SESSION_XP_FROM_TESTS / SUCCESSFULLY_RUN_FILES))
        display_combos_xp=$((SESSION_XP_FROM_COMBOS / SUCCESSFULLY_RUN_FILES))
        display_achievements_xp=$((SESSION_XP_FROM_ACHIEVEMENTS / SUCCESSFULLY_RUN_FILES))
        display_original_xp=$((original_session_xp / SUCCESSFULLY_RUN_FILES))
        # Penalty is NOT divided - it's applied after bulk reduction, making it harsh
    fi

    # Calculate final display XP after penalty
    local display_total_xp=$((display_original_xp > display_bulk_penalty ? display_original_xp - display_bulk_penalty : 0))

    # Always show XP breakdown with failures section
    if [ "$TOTAL_BULK_FAILURES" -gt 0 ]; then
        echo -e "💰 Session XP: ${YELLOW}$(format_number_with_commas $display_total_xp)${NC} | 🧪 From Tests: ${GREEN}$(format_number_with_commas $display_tests_xp)${NC} | 🔥 From Combos: ${ORANGE}$(format_number_with_commas $display_combos_xp)${NC} | 💀 From Failures: ${RED}-$(format_number_with_commas $display_bulk_penalty)${NC}"
        if [ "$display_original_xp" -gt 0 ] && [ "$display_bulk_penalty" -gt 0 ]; then
            echo -e "${GRAY}   Original XP: $(format_number_with_commas $display_original_xp) → Final XP: $(format_number_with_commas $display_total_xp) (${TOTAL_BULK_FAILURES} failures penalty)${NC}"
        fi
    else
        # No failures - show 0 penalty instead of achievements
        echo -e "💰 Session XP: ${YELLOW}$(format_number_with_commas $display_total_xp)${NC} | 🧪 From Tests: ${GREEN}$(format_number_with_commas $display_tests_xp)${NC} | 🔥 From Combos: ${ORANGE}$(format_number_with_commas $display_combos_xp)${NC} | 💀 From Failures: ${RED}-0${NC}"
    fi
    echo -e "${PURPLE}$greeting...${NC}"
    echo -e "    $greeting_context"

    # Analyze test results and provide technical feedback with thematic presentation
    if [ "$prev_timestamp" = "never" ]; then
        # First run - Initialize tracking
        echo -e "${CYAN}✨ TEST FRAMEWORK INITIALIZED${NC}"
        echo -e "    ${FADED}Sisyphus continuous improvement tracking started${NC}"

        if [ "$total_curr_issues" -eq 0 ]; then
            local symbol=$(get_narrative_symbol)
            echo -e "${GREEN}$symbol THE ARCHITECT'S DREAM! From the first stone, perfection emerges${NC}"
            echo -e "    ${FADED}'Impossible! Even I, Daedalus, master of all crafts, have never seen such initial precision!' -Daedalus${NC}"
        elif [ "$total_curr_issues" -le 5 ]; then
            echo -e "${PURPLE}🏛️ THE FOUNDATION IS LAID! $total_curr_issues minor flaws await the master's touch${NC}"
            echo -e "    ${FADED}Sisyphus begins his eternal work: each push of the boulder, each fix of the code.${NC}"
        else
            echo -e "${RED}🌊 THE CHAOS OF CREATION! $total_curr_issues primordial errors swirl in the architectural void${NC}"
            echo -e "    ${FADED}From such chaos, heroes are born. Let the great work begin, Sisyphus.${NC}"
        fi
    elif [ "$total_change" -lt 0 ]; then
        # IMPROVEMENT DETECTED - Technical progress with gamification
        local improvement=$((total_prev_issues - total_curr_issues))
        local symbol=$(get_narrative_symbol)

        if [ "$improvement" -ge 10 ]; then
            local symbol=$(get_narrative_symbol)
            echo -e "${GREEN}$symbol DAEDALUS WEEPS WITH JOY! The master architect witnesses $improvement flaws vanish like morning mist${NC}"
            echo -e "    ${FADED}'By the gods, Sisyphus! In one session you have achieved what I thought impossible!'${NC}"
            echo -e "${BOLD_WHITE}🏛️ LEGENDARY BREAKTHROUGH: The Labyrinth trembles as its deepest mysteries yield to your will${NC}"
        elif [ "$improvement" -ge 5 ]; then
            local symbol=$(get_narrative_symbol)
            echo -e "${GREEN}$symbol THE BOULDER ROLLS UPHILL! $improvement grievous errors crumble beneath your relentless effort${NC}"
            echo -e "    ${FADED}'The gods themselves take notice when mortals achieve such systematic triumph over chaos'${NC}"
            echo -e "${BOLD_WHITE}⚡ DIVINE RECOGNITION: Even Zeus pauses his thunderbolts to observe your mastery${NC}"
        elif [ "$improvement" -ge 2 ]; then
            local symbol=$(get_narrative_symbol)
            echo -e "${GREEN}$symbol SISYPHEAN PERSISTENCE! Each of $improvement flaws falls to your methodical assault${NC}"
            echo -e "    ${FADED}'This is the true nature of the curse: not the pushing, but the perfecting'${NC}"
            echo -e "${BOLD_WHITE}🔥 THE FORGE BURNS BRIGHT: Your dedication transforms base code into architectural gold${NC}"
        else
            echo -e "${GREEN}⚡ THE LABYRINTH YIELDS! One more passage cleared, one more mystery solved${NC}"
            echo -e "    ${FADED}'In the endless work, Sisyphus, each small victory is a rebellion against fate'${NC}"
        fi

        # Story-driven streak tracking with existential weight
        if [ "$current_streak" -ge 5 ]; then
            echo -e "${PURPLE}🌀 THE ETERNAL RHYTHM! For $current_streak cycles, the boulder has rolled only upward${NC}"
            echo -e "    ${FADED}Daedalus whispers: 'You have found the secret, Sisyphus. Embrace the absurd, and it becomes divine.'${NC}"
        elif [ "$current_streak" -ge 3 ]; then
            echo -e "${CYAN}⛓️ BREAKING THE PATTERN! $current_streak successive triumphs over the Labyrinth's chaos${NC}"
            echo -e "    ${FADED}The chains of your curse grow lighter with each victory. Purpose emerges from punishment.${NC}"
        elif [ "$current_streak" -ge 2 ]; then
            echo -e "${YELLOW}🪨 THE BOULDER LISTENS! $current_streak pushes without it rolling back down${NC}"
            echo -e "    ${FADED}You begin to understand: the stone is not your enemy, but your teacher.${NC}"
        fi

        # Narrative-driven technical achievements that advance the story
        if [ "$compile_change" -gt 0 ]; then
            echo -e "${CYAN}🏗️ DAEDALUS'S BLUEPRINTS PERFECTED! $compile_change architectural flaws corrected${NC}"
            echo -e "    ${FADED}'The foundation stones no longer crack. Your craftsmanship rivals my own.' -Daedalus${NC}"
        fi
        if [ "$runtime_change" -gt 0 ]; then
            echo -e "${ORANGE}🐂 MINOTAUR'S RAGE CONTAINED! $runtime_change beast(s) no longer rampage through memory${NC}"
            echo -e "    ${FADED}The Labyrinth's walls hold firm. No creature shall escape your perfect prison.${NC}"
        fi
        if [ "$pass_change" -gt 0 ]; then
            echo -e "${GREEN}🌟 THE THREADS OF ARIADNE! +$pass_change more paths now lead to salvation${NC}"
            echo -e "    ${FADED}Heroes yet unborn will find their way through the maze because of your work.${NC}"
        fi
    elif [ "$total_change" -gt 0 ]; then
        # REGRESSION DETECTED - Technical analysis with motivational framing
        local setback=$total_change

        if [ "$setback" -le 2 ]; then
            echo -e "${CYAN}🌊 THE LABYRINTH SHIFTS! $setback new passage(s) have twisted in the shadows${NC}"
            echo -e "    ${FADED}'The maze resists us, Sisyphus. It learns, it adapts, it fights back.' -Daedalus${NC}"
            echo -e "${BOLD_WHITE}🗿 THE ARCHITECT'S TASK: Reshape the rebellious stone with your chisel${NC}"
        elif [ "$setback" -le 5 ]; then
            echo -e "${ORANGE}🐍 THE HYDRA'S REVENGE! Cut off $setback heads, yet more problems sprout from the wound${NC}"
            echo -e "    ${FADED}The curse reveals its true nature: each solution births new complexities.${NC}"
            echo -e "${BOLD_WHITE}⚔️ HERO'S TRIAL: Face the growing chaos with Sisyphean determination${NC}"
        else
            echo -e "${RED}🌀 THE LABYRINTH REBELS! $setback new torments emerge from the chaotic depths${NC}"
            echo -e "    ${FADED}'It has become aware, Sisyphus. The maze now actively opposes its own completion.' -Daedalus${NC}"
            echo -e "${BOLD_WHITE}👑 MINOS'S JUDGMENT: The King demands answers for this architectural insurrection${NC}"
        fi

        # Narrative progress that maintains hope despite setbacks
        if [ "$pass_change" -gt 0 ]; then
            echo -e "${GREEN}🕊️ ICARUS'S HOPE! +$pass_change small victories shine through the gathering storm${NC}"
            echo -e "    ${FADED}Even in regression, progress persists. The work continues, the dream endures.${NC}"
        fi
        if [ "$efficiency_change" -gt 0 ]; then
            echo -e "${PURPLE}⚡ PROMETHEUS'S GIFT! The code burns ${efficiency_change}x brighter than before${NC}"
            echo -e "    ${FADED}You have stolen fire from the gods of inefficiency and given it to mortals.${NC}"
        fi
    else
        # NO CHANGE - Mythological stability analysis with story progression
        declare -a stable_messages=(
            "🏛️  The walls have not shifted. The corridors have not changed their shape."
            "🪨 The weight of the task neither lessens nor grows."
            "👑 From his balcony, King Minos observes no change."
            "🧠 Daedalus stares at the blueprints, a day's work resulting in no new progress."
            "🏛️  The scales of fate have neither tipped towards triumph nor disaster."
        )
        # Select and display a random stability message
        local selected_stable="${stable_messages[RANDOM % ${#stable_messages[@]}]}"
        IFS='|' read -r message context <<< "$selected_stable"
        echo -e "${CYAN}$message${NC}"

        if [ "$total_curr_issues" -eq 0 ]; then
            # Perfect state - existential achievement messages
            local perfect_messages=(
                "${GREEN}🏛️ THE ETERNAL MOMENT! In this instant, Sisyphus, the boulder rests at the summit${NC}|'Impossible,' whispers Daedalus. 'We have achieved what the gods deemed eternal punishment.'"
                "${GREEN}👑 MINOS'S SILENT APPROVAL! The King stares in wonder at the perfect Labyrinth${NC}|For once, the tyrant has no demands. Your work has exceeded even his cruel imagination."
                "${GREEN}🌟 THE ABSURD TRANSCENDED! In perfecting the meaningless, you have found meaning${NC}|This is the secret Camus never told: embrace the absurd completely, and it becomes sacred."
                "${GREEN}🐂 THE MINOTAUR BOWS! Even the beast recognizes the perfection of its prison${NC}|In the deepest chamber, a mournful lowing becomes something almost like... gratitude?"
            )
            local selected_perfect="${perfect_messages[RANDOM % ${#perfect_messages[@]}]}"
            IFS='|' read -r message context <<< "$selected_perfect"
            echo -e "$message"
            echo -e "    ${FADED}$context${NC}"
        elif [ "$total_curr_issues" -eq 1 ]; then
            # Single issue - The final moment
            if [ "$COMPILE_ERRORS" -gt 0 ]; then
                echo -e "${YELLOW}👑 MINOS' FINAL DECREE! One blueprint flaw blocks the royal seal${NC}"
                echo -e "    ${FADED}King Minos awaits: fix the final architectural error to complete the labyrinth${NC}"
            elif [ "$RUNTIME_ERRORS" -gt 0 ]; then
                echo -e "${YELLOW}👑 MINOS' FINAL DECREE! One beast prowls the completed corridors${NC}"
                echo -e "    ${FADED}King Minos commands: slay the final creature to secure the labyrinth${NC}"
            else
                echo -e "${YELLOW}👑 MINOS' FINAL DECREE! One corridor remains unexplored${NC}"
                echo -e "    ${FADED}King Minos demands: navigate the final passage to prove the maze${NC}"
            fi
        elif [ "$total_curr_issues" -le 3 ]; then
            # Few issues - existential near-completion
            local near_completion_messages=(
                "${CYAN}🏗️ DAEDALUS INSPECTS! 'Only $total_curr_issues final touches remain on my masterpiece'${NC}|Master architect nods approvingly - the labyrinth nears completion"
                "${CYAN}🗡️  THESEUS APPROACHES! The hero senses only $total_curr_issues obstacles ahead${NC}|Legendary warrior prepares - victory lies just beyond these final trials"
                "${CYAN}👑 A NOD FROM MINOS! The King sees but $total_curr_issues minor details to perfect in his Labyrinth${NC}|The final judgment is near. Achieve perfection to satisfy the decree of Crete."
                "${CYAN}🪨 THE BURDEN LIGHTENS! Sisyphus, your task feels easier now; only $total_curr_issues steps remain${NC}|The summit is in sight. One final push will grant you a moment's peace."
                "${CYAN}🗿 THE CHISEL'S FINAL STROKES! Only $total_curr_issues imperfections mar the eternal work${NC}|'I begin to understand,' Daedalus murmurs, 'why the gods chose you for this task.'"
                "${CYAN}👑 THE KING'S SOFTENING GAZE! Even Minos seems moved by the near-completion of his terrible dream${NC}|'Perhaps,' he whispers, 'I have been the true monster all along.'"
                "${CYAN}🪨 THE BOULDER'S WHISPER! The stone itself seems eager to reach the summit - only $total_curr_issues pushes remain${NC}|You realize: the mountain was never your enemy. It was waiting to be climbed."

            )
            local selected_near="${near_completion_messages[RANDOM % ${#near_completion_messages[@]}]}"
            IFS='|' read -r message context <<< "$selected_near"
            echo -e "$message"
            echo -e "    ${FADED}$context${NC}"
        elif [ "$total_curr_issues" -le 6 ]; then
            # Moderate issues - philosophical middle ground
            local moderate_messages=(
                "${PURPLE}🏛️  THE LABYRINTH'S CALL! The winding paths present $total_curr_issues fresh puzzles to be solved${NC}|The master craftsman, Daedalus, watches to see how you navigate the complexity."
                "${PURPLE}👑 A REPORT FOR KING MINOS! His scouts have noted $total_curr_issues unresolved issues within the maze${NC}|The King expects progress. Each fix brings the Labyrinth closer to his standard of perfection."
                "${PURPLE}🪨 A FAMILIAR WEIGHT... Sisyphus, your boulder feels heavier with $total_curr_issues new imperfections to address${NC}|The struggle is constant, but so is your strength. Push onward; the summit is earned, not given."
                "${PURPLE}🐂 THE MINOTAUR'S GROWL! The beast senses $total_curr_issues weaknesses in its prison walls${NC}|The Labyrinth must be flawless to contain its prisoner. Reinforce the logic and seal the exits."
                "${PURPLE}🌀 THE LABYRINTH BREATHES! $total_curr_issues living contradictions pulse through its corridors${NC}|'It is not malevolent,' Daedalus realizes. 'It is just... alive. And it wants to live perfectly.'"
                "${PURPLE}👑 MINOS'S INTERNAL STRUGGLE! The King counts $total_curr_issues reasons to continue, and $total_curr_issues reasons to stop${NC}|You see him in the courtyard, weeping. Even tyrants know the weight of their choices."
                "${PURPLE}🪨 THE MIDDLE DISTANCE! Sisyphus, neither at the bottom nor the top, contemplates $total_curr_issues truths${NC}|In this space between despair and triumph, you find something unexpected: contentment."
            )
            local selected_moderate="${moderate_messages[RANDOM % ${#moderate_messages[@]}]}"
            IFS='|' read -r message context <<< "$selected_moderate"
            echo -e "$message"
            echo -e "    ${FADED}$context${NC}"
        else
            # Many issues - existential overwhelming
            local epic_quest_messages=(
                "${RED}👑 A FURIOUS DECREE FROM MINOS! 'This Labyrinth is overrun with $total_curr_issues flaws! Correct them, Sisyphus, or face the full wrath of Crete!'${NC}|The King's patience wears thin. The scale of this task is a trial in itself."
                "${RED}🏗️  THE ARCHITECT'S DESPAIR! Daedalus cries out, '$total_curr_issues structural failures threaten to collapse my great work!'${NC}|The very foundations of the Labyrinth are compromised. A master builder is needed to prevent total ruin."
                "${RED}🪨 THE BOULDER'S TRUE WEIGHT! The path to the summit is blocked by $total_curr_issues immense obstacles${NC}|This is your curse and your purpose. Push onward, for the task is eternal and the summit is but a brief respite."
                "${RED}🧹 THE AUGEAN STABLES! The Labyrinth is flooded with $total_curr_issues sources of filth and chaos${NC}|A task worthy of Heracles himself. It is time for a great cleansing to restore order to the maze."
                "${RED}🌪️  THE GATES OF TARTARUS ARE BREACHED! $total_curr_issues chaotic spirits have been unleashed within the walls${NC}|A hero's greatest quest is to face the underworld. Bring order to the chaos and prove your mastery."
                "${RED}🌀 THE PRIMORDIAL CHAOS! $total_curr_issues contradictions swirl in the void before creation${NC}|'This is how the world began,' Daedalus whispers in awe. 'With exactly this much beautiful confusion.'"
                "${RED}🗿 THE UNCARVED BLOCK! $total_curr_issues infinite possibilities await the sculptor's vision${NC}|You stand before the raw marble of existence. Every error is a choice, every fix a small act of creation."
                "${RED}🪨 THE ETERNAL BEGINNING! Sisyphus faces $total_curr_issues reasons why the boulder must be pushed${NC}|But you smile. For in the infinite task, you have found infinite purpose. Let the work begin again."
                "${RED}🔥 THE FORGE OF WORLDS! $total_curr_issues elements await transformation in Hephaestus's flames${NC}|In the beginning was chaos. In the end will be beauty. You are the alchemist of the between."

            )
            local selected_epic="${epic_quest_messages[RANDOM % ${#epic_quest_messages[@]}]}"
            IFS='|' read -r message context <<< "$selected_epic"
            echo -e "$message"
            echo -e "    ${FADED}$context${NC}"
        fi
    fi


    local final_messages=(
        "✨ Your code is your Sword, your logic your Shield!"
        "🌟 Each keystroke echoes through the halls of Olympus!"
        "🏆 Heroes are born in moments of impossible triumph!"
        "📜 'Know thyself' - Socrates speaks through your structured code!"
        "🧘 'You have power over your mind - and your code' - Marcus Aurelius guides your focus!"
        "📜 'The unexamined code is not worth running' - Socratic programming!"
        "🎯 'Excellence is never an accident' - Aristotle approves your discipline!"
        "📚 'The only true wisdom is knowing you know nothing' - Socratic humility!"
        "🏛️ 'We are what we repeatedly do' - Aristotelian habit formation!"
        "🔥 'From small sparks, great fires kindle' - Heraclitean transformation!"
        "🌊 'No developer ever runs the same code twice' - Heraclitean flow!"
        "💎 'Virtue is its own reward' - Stoic satisfaction in clean code!"
        "🏺 'The whole is greater than the sum of its parts' - Aristotelian architecture!"
        "✨ 'Think like a mountain' - Marcus Aurelius teaches patient debugging!"
        "🦉 'The fox knows many things, but the hedgehog knows one big thing' - Archilochus!"
        "🛡️ 'Fortune favors the bold' - but preparation favors the coder!"
        "🏛️ 'The measure of a man is what he does with power' - Plato's responsibility!"
        "🌿 'Every oak tree was once an acorn that held its ground' - Patience pays!"
        "✨ 'First understand the riddle, then craft the solution' - Wisdom of the ancients!"
        "🧠 'The most elegant temple needs no ornaments' - Athena's minimalist architecture!"
        "🔧 'Build it to stand, perfect it to last, optimize it to soar' - Hephaestus's forge trinity!"
        "🎯 'True mastery lies in making the complex appear simple' - Apollo's divine sophistication!"
        "🏗️ 'Scrolls are read by many, but written by few' - Daedalus's architectural clarity!"
        "💡 'Strategy wins wars, not the swing of swords' - Athena's tactical insight!"
        "🌊 'The best prophecy is one that prevents disaster' - Oracle's error wisdom!"
        "💎 'Rushing to perfection often ruins the foundation' - Aristotle's cautious balance!"
        "🎨 'Masterpieces reveal the soul of their creator' - Pygmalion's caring craft!"
    )

    # Final status assessment with technical recommendations
    local victory_shown=false
    if [ "$total_curr_issues" -eq 0 ]; then
        # Perfect state - epic victory declarations with new characters
        local victory_messages=(
            "${GREEN}🎉 MINOTAUR DEFEATED! The labyrinth guardian bows before your flawless code${NC}|All compilation successful, zero runtime crashes, perfect test results|${BOLD_WHITE}🌟 HEROIC LEGEND! Bards will sing of your debugging mastery!${NC}"
            "${GREEN}🏛️ DAEDALUS ADMIRES THE PERFECTION! 'The blueprints are flawless. The structure is absolute. I could not have built it better myself.'${NC}|The master craftsman inspects your work and finds no fault. The Labyrinth is a mathematical marvel.|${BOLD_WHITE}✨ ARCHITECT'S BLESSING! Your logic has surpassed the creator's vision!${NC}"
            "${GREEN}👑 KING MINOS'S ULTIMATE TREASURE! The King declares, 'This Labyrinth is now the most prized possession of Crete! Its flawless design will be legendary.'${NC}|Your work has not only met the royal standard but has become a source of immense pride for the kingdom.|${BOLD_WHITE}💎 THE JEWEL OF CRETE! Your code is now a treasure of mythological proportions!${NC}"
            "${GREEN}🪨 THE BOULDER RESTS! At the summit, Sisyphus finds a moment of perfect, silent peace${NC}|All labors are complete. The curse is momentarily lifted by the profound stillness of perfection.|${BOLD_WHITE}✨ ETERNAL TASK, PERFECTED! You have achieved the impossible. Rest... until the work begins anew.${NC}"
        )
        local selected_victory="${victory_messages[RANDOM % ${#victory_messages[@]}]}"
        IFS='|' read -r message1 message2 message3 <<< "$selected_victory"
        echo -e "$message1"
        echo -e "    ${FADED}$message2${NC}"
        echo -e "$message3"
        victory_shown=true
    elif [ "$total_curr_issues" -le 3 ]; then
        # Near completion - single line with Sisyphus-themed breakdown
        local final_breakdowns=(
            "$COMPILE_ERRORS blueprint flaws, $RUNTIME_ERRORS lurking beasts, ${#FAILED_FILES[@]} blocked passages"
            "$COMPILE_ERRORS structural cracks, $RUNTIME_ERRORS memory hydras, ${#FAILED_FILES[@]} maze dead-ends"
            "$COMPILE_ERRORS foundation gaps, $RUNTIME_ERRORS prowling minotaurs, ${#FAILED_FILES[@]} corridor puzzles"
            "$COMPILE_ERRORS architectural errors, $RUNTIME_ERRORS segfault serpents, ${#FAILED_FILES[@]} labyrinth riddles"
            "$COMPILE_ERRORS broken blueprints, $RUNTIME_ERRORS crash chimeras, ${#FAILED_FILES[@]} hidden doorways"
            "$COMPILE_ERRORS design flaws, $RUNTIME_ERRORS wild centaurs, ${#FAILED_FILES[@]} secret chambers"
            "$COMPILE_ERRORS cracked foundations, $RUNTIME_ERRORS buffer cyclops, ${#FAILED_FILES[@]} twisted pathways"
            "$COMPILE_ERRORS incomplete plans, $RUNTIME_ERRORS memory griffins, ${#FAILED_FILES[@]} unsolved mysteries"
            "$COMPILE_ERRORS build failures, $RUNTIME_ERRORS execution crashes, ${#FAILED_FILES[@]} test failures"
            "$COMPILE_ERRORS syntax errors, $RUNTIME_ERRORS memory errors, ${#FAILED_FILES[@]} logic errors"
            "$COMPILE_ERRORS linker issues, $RUNTIME_ERRORS runtime crashes, ${#FAILED_FILES[@]} assertion failures"
        )
        local selected_breakdown="${final_breakdowns[RANDOM % ${#final_breakdowns[@]}]}"
        colored_breakdown=$(echo "$selected_breakdown" | sed -E \
            -e "s/([0-9]+) (blueprint flaws|structural cracks|foundation gaps|architectural errors|broken blueprints|design flaws|cracked foundations|incomplete plans|build failures|syntax errors|linker issues)/\x1b[0;31m\1 \2\x1b[0m/g" \
            -e "s/([0-9]+) (lurking beasts|memory hydras|prowling minotaurs|segfault serpents|crash chimeras|wild centaurs|buffer cyclops|memory griffins|execution crashes|memory errors|runtime crashes)/\x1b[0;93m\1 \2\x1b[0m/g" \
            -e "s/([0-9]+) (blocked passages|maze dead-ends|corridor puzzles|labyrinth riddles|hidden doorways|secret chambers|twisted pathways|unsolved mysteries|test failures|logic errors|assertion failures)/\x1b[1;33m\1 \2\x1b[0m/g")
        echo -e "    ${FADED}Final debugging needed: $colored_breakdown${NC}"
    elif [ "$total_curr_issues" -le 5 ]; then
        # Moderate progress - single line with Sisyphus-themed breakdown
        local moderate_breakdowns=(
            "$COMPILE_ERRORS construction delays, $RUNTIME_ERRORS territorial beasts, ${#FAILED_FILES[@]} unexplored corridors"
            "$COMPILE_ERRORS workshop mishaps, $RUNTIME_ERRORS roaming monsters, ${#FAILED_FILES[@]} mapping challenges"
            "$COMPILE_ERRORS blueprint revisions, $RUNTIME_ERRORS beast encounters, ${#FAILED_FILES[@]} navigation puzzles"
            "$COMPILE_ERRORS engineering setbacks, $RUNTIME_ERRORS creature invasions, ${#FAILED_FILES[@]} path obstructions"
            "$COMPILE_ERRORS structural issues, $RUNTIME_ERRORS monster nests, ${#FAILED_FILES[@]} corridor blockages"
            "$COMPILE_ERRORS design challenges, $RUNTIME_ERRORS beast territories, ${#FAILED_FILES[@]} maze complications"
            "$COMPILE_ERRORS building problems, $RUNTIME_ERRORS lurking dangers, ${#FAILED_FILES[@]} exploration hurdles"
            "$COMPILE_ERRORS assembly errors, $RUNTIME_ERRORS beast patrols, ${#FAILED_FILES[@]} passage mysteries"
            "$COMPILE_ERRORS compilation errors, $RUNTIME_ERRORS segmentation faults, ${#FAILED_FILES[@]} failing tests"
            "$COMPILE_ERRORS dependency problems, $RUNTIME_ERRORS memory violations, ${#FAILED_FILES[@]} broken functions"
            "$COMPILE_ERRORS header conflicts, $RUNTIME_ERRORS access violations, ${#FAILED_FILES[@]} algorithm bugs"
        )
        local selected_breakdown="${moderate_breakdowns[RANDOM % ${#moderate_breakdowns[@]}]}"
        colored_breakdown=$(echo "$selected_breakdown" | sed -E \
            -e "s/([0-9]+) (construction delays|workshop mishaps|blueprint revisions|engineering setbacks|structural issues|design challenges|building problems|assembly errors|compilation errors|dependency problems|header conflicts)/\x1b[0;31m\1 \2\x1b[0m/g" \
            -e "s/([0-9]+) (territorial beasts|roaming monsters|beast encounters|creature invasions|monster nests|beast territories|lurking dangers|beast patrols|segmentation faults|memory violations|access violations)/\x1b[0;93m\1 \2\x1b[0m/g" \
            -e "s/([0-9]+) (unexplored corridors|mapping challenges|navigation puzzles|path obstructions|corridor blockages|maze complications|exploration hurdles|passage mysteries|failing tests|broken functions|algorithm bugs)/\x1b[1;33m\1 \2\x1b[0m/g")
        echo -e "    ${FADED}Development work ahead: $colored_breakdown${NC}"
    else
        # Many issues - single line with Sisyphus-themed breakdown
        local major_breakdowns=(
            "$COMPILE_ERRORS blueprint disasters, $RUNTIME_ERRORS beast invasions, ${#FAILED_FILES[@]} labyrinth mysteries"
            "$COMPILE_ERRORS foundation collapses, $RUNTIME_ERRORS monster hordes, ${#FAILED_FILES[@]} corridor chaos"
            "$COMPILE_ERRORS structural catastrophes, $RUNTIME_ERRORS creature stampedes, ${#FAILED_FILES[@]} maze confusion"
            "$COMPILE_ERRORS design meltdowns, $RUNTIME_ERRORS beast rampages, ${#FAILED_FILES[@]} passage pandemonium"
            "$COMPILE_ERRORS workshop explosions, $RUNTIME_ERRORS hydra attacks, ${#FAILED_FILES[@]} navigation nightmares"
            "$COMPILE_ERRORS architectural crises, $RUNTIME_ERRORS monster sieges, ${#FAILED_FILES[@]} corridor catastrophes"
            "$COMPILE_ERRORS blueprint chaos, $RUNTIME_ERRORS beast rebellions, ${#FAILED_FILES[@]} labyrinth upheaval"
            "$COMPILE_ERRORS construction disasters, $RUNTIME_ERRORS creature fury, ${#FAILED_FILES[@]} maze madness"
            "$COMPILE_ERRORS engineering failures, $RUNTIME_ERRORS beast riots, ${#FAILED_FILES[@]} passage turmoil"
            "$COMPILE_ERRORS foundational cracks, $RUNTIME_ERRORS monster mayhem, ${#FAILED_FILES[@]} exploration disasters"
            "$COMPILE_ERRORS build system failures, $RUNTIME_ERRORS program crashes, ${#FAILED_FILES[@]} test suite failures"
            "$COMPILE_ERRORS preprocessor errors, $RUNTIME_ERRORS stack overflows, ${#FAILED_FILES[@]} validation errors"
            "$COMPILE_ERRORS linking failures, $RUNTIME_ERRORS heap corruption, ${#FAILED_FILES[@]} regression bugs"
        )
        local selected_breakdown="${major_breakdowns[RANDOM % ${#major_breakdowns[@]}]}"
        colored_breakdown=$(echo "$selected_breakdown" | sed -E \
            -e "s/([0-9]+) (blueprint disasters|foundation collapses|structural catastrophes|design meltdowns|workshop explosions|architectural crises|blueprint chaos|construction disasters|engineering failures|foundational cracks|build system failures|preprocessor errors|linking failures)/\x1b[0;31m\1 \2\x1b[0m/g" \
            -e "s/([0-9]+) (beast invasions|monster hordes|creature stampedes|beast rampages|hydra attacks|monster sieges|beast rebellions|creature fury|beast riots|monster mayhem|program crashes|stack overflows|heap corruption)/\x1b[0;93m\1 \2\x1b[0m/g" \
            -e "s/([0-9]+) (labyrinth mysteries|corridor chaos|maze confusion|passage pandemonium|navigation nightmares|corridor catastrophes|labyrinth upheaval|maze madness|passage turmoil|exploration disasters|test suite failures|validation errors|regression bugs)/\x1b[1;33m\1 \2\x1b[0m/g")
        echo -e "    ${FADED}Major debugging work: $colored_breakdown${NC}"
    fi

    # Display current development focus if there are issues
    if [ "$total_curr_issues" -gt 0 ] && [ -n "$curr_challenge" ]; then
        echo -e "${BOLD_WHITE}CURRENT QUEST:${NC}"

        if [[ "$curr_challenge" == *"Final Requiem"* ]] || [[ "$curr_challenge" == *"Mount Olympus"* ]]; then
            # Single issue remaining - final boss variety
            local final_quests=(
                "${YELLOW}👑 ROYAL SUMMONS: King Minos awaits your final demonstration of mastery${NC}|The throne room doors open only when perfection is achieved"
                "${YELLOW}🏛️  LABYRINTH COMPLETION: Daedalus awaits your final architectural touch${NC}|The master craftsman gestures - one last corridor blocks the maze's perfection"
                "${YELLOW}✨ THESEUS' FINAL STEP: The hero stands before the last chamber's door${NC}|Thread in hand, one final challenge separates you from conquering the labyrinth"
                "${YELLOW}🌟 MINOTAUR'S LAST ROAR: The beast retreats to its final stronghold${NC}|Echoes fade through empty corridors - one last passage guards the labyrinth's heart"
            )
            local selected_final="${final_quests[RANDOM % ${#final_quests[@]}]}"
            IFS='|' read -r quest_msg quest_context <<< "$selected_final"
            echo -e "    $quest_msg"
            echo -e "    ${FADED}$quest_context${NC}"

            # Randomly suggest a specific file to fix from all failed tests
            local all_failed_suggestions=()
            for failed in "${FAILED_FILES[@]}"; do
                all_failed_suggestions+=("$failed")
            done
            for compile_error in "${COMPILE_ERROR_FILES[@]}"; do
                all_failed_suggestions+=("$compile_error:COMPILE_ERROR")
            done
            for runtime_error in "${RUNTIME_ERROR_FILES[@]}"; do
                all_failed_suggestions+=("$runtime_error:RUNTIME_ERROR")
            done

            if [ ${#all_failed_suggestions[@]} -gt 0 ]; then
                local random_failed_suggestion="${all_failed_suggestions[RANDOM % ${#all_failed_suggestions[@]}]}"
                local suggested_file=$(echo "$random_failed_suggestion" | cut -d':' -f1)
                local suggested_target=$(echo "$random_failed_suggestion" | cut -d':' -f2)
                local file_path=$(map_target_to_file_path "$suggested_target")
                echo -e "    ${CYAN}🎯 Complete the task: ${BOLD_WHITE}$suggested_file${NC}"
                echo -e "    Execute: ${YELLOW}make $suggested_target${NC} | ${CYAN}View Test:${NC} \e]8;;file://$(pwd)/$file_path\e\\Click Here\e]8;;\e\\"
            fi

        elif [[ "$curr_challenge" == *"Workshop"* ]] || [[ "$curr_challenge" == *"Daedalus"* ]]; then
            # Compile error quests - blueprint/architecture focus
            echo -e "    ${YELLOW}🏗️  REPAIR BLUEPRINTS: Daedalus needs help with ${RED}${#COMPILE_ERROR_FILES[@]} compilation error${YELLOW} files${NC}"

            # Randomly suggest a specific file to fix
            if [ ${#COMPILE_ERROR_FILES[@]} -gt 0 ]; then
                local random_compile_error="${COMPILE_ERROR_FILES[RANDOM % ${#COMPILE_ERROR_FILES[@]}]}"
                local suggested_file=$(echo "$random_compile_error" | cut -d':' -f1)
                local suggested_target=$(echo "$random_compile_error" | cut -d':' -f2)
                local file_path=$(map_target_to_file_path "$suggested_target")
                echo -e "    ${CYAN}📍 Focus on: ${BOLD_WHITE}$suggested_file${NC}"
                echo -e "    Debug: ${YELLOW}make $suggested_target${NC} | ${CYAN}View Test:${NC} \e]8;;file://$(pwd)/$file_path\e\\Click Here\e]8;;\e\\"
            fi

            local blueprint_tips=(
                "Check #include statements - missing headers block construction"
                "Verify function declarations match implementations exactly"
                "Examine linker flags - libraries must be properly connected"
                "Review Makefile dependencies for correct build order"
                "Search for typos in function names and variable declarations"
                "Ensure all required libraries are installed and linked"
            )
            local selected_tip="${blueprint_tips[RANDOM % ${#blueprint_tips[@]}]}"
            echo -e "    ${FADED}Engineering wisdom: $selected_tip${NC}"

        elif [[ "$curr_challenge" == *"Hydra"* ]] || [[ "$curr_challenge" == *"Beasts"* ]] || [[ "$curr_challenge" == *"runtime"* ]]; then
            # Runtime error quests - monster hunting
            echo -e "    ${YELLOW}🗡️  SLAY MYTHICAL BEASTS: Clear ${ORANGE}${#RUNTIME_ERROR_FILES[@]} dangerous runtime error files${YELLOW} from the labyrinth depths${NC}"

            # Randomly suggest a specific file to fix
            if [ ${#RUNTIME_ERROR_FILES[@]} -gt 0 ]; then
                local random_runtime_error="${RUNTIME_ERROR_FILES[RANDOM % ${#RUNTIME_ERROR_FILES[@]}]}"
                local suggested_file=$(echo "$random_runtime_error" | cut -d':' -f1)
                local suggested_target=$(echo "$random_runtime_error" | cut -d':' -f2)
                local file_path=$(map_target_to_file_path "$suggested_target")
                echo -e "    ${CYAN}🎯 Hunt the beast in:${NC} ${YELLOW}make $suggested_target${NC} | ${CYAN}View Test:${NC} \e]8;;file://$(pwd)/$file_path\e\\Click Here\e]8;;\e\\"
            fi

            local monster_tips=(
                "Run with valgrind to track memory corruption and leaks"
                "Initialize all pointers to NULL before use"
                "Check array bounds - ensure indices stay within allocated memory"
                "Verify malloc/free pairs - every allocation needs deallocation"
                "Use debugger breakpoints to trace execution before crashes"
                "Add printf statements to track variable values before segfaults"
                "Check for uninitialized variables causing random behavior"
            )
            local selected_tip="${monster_tips[RANDOM % ${#monster_tips[@]}]}"
            echo -e "    ${FADED}Monster hunting tip: $selected_tip${NC}"

        elif [[ "$curr_challenge" == *"Labyrinth"* ]] || [[ "$curr_challenge" == *"Minotaur"* ]]; then
            # Test failure quests - exploration/navigation focus
            echo -e "    ${YELLOW}🧭 EXPLORE LABYRINTH: Navigate ${YELLOW}${#FAILED_FILES[@]} test failure corridors${YELLOW} and solve their mysteries${NC}"

            # Randomly suggest a specific file to fix
            if [ ${#FAILED_FILES[@]} -gt 0 ]; then
                local random_failed_test="${FAILED_FILES[RANDOM % ${#FAILED_FILES[@]}]}"
                local suggested_file=$(echo "$random_failed_test" | cut -d':' -f1)
                local suggested_target=$(echo "$random_failed_test" | cut -d':' -f2)
                local file_path=$(map_target_to_file_path "$suggested_target")
                echo -e "    ${CYAN}🗺️  Start exploring: ${BOLD_WHITE}$suggested_file${NC}"
                echo -e "    Explore: ${YELLOW}make $suggested_target${NC} | ${CYAN}View Test:${NC} \e]8;;file://$(pwd)/$file_path\e\\Click Here\e]8;;\e\\"
            fi

            local exploration_tips=(
                "Compare expected vs actual outputs line by line"
                "Add debug prints to trace execution flow through your logic"
                "Test edge cases: empty inputs, boundary values, null data"
                "Verify loop conditions - check off-by-one errors"
                "Examine conditional statements for missing or wrong logic"
                "Use assertion messages to understand what the test expects"
                "Break complex functions into smaller, testable pieces"
            )
            local selected_exploration="${exploration_tips[RANDOM % ${#exploration_tips[@]}]}"
            echo -e "    ${FADED}Explorer's wisdom: $selected_exploration${NC}"

        else
            # Fallback for any other challenge types
            echo -e "    ${YELLOW}⚡ DIVINE TRIAL: The gods test your coding resolve with complex challenges${NC}"
            local general_tips=(
                "Debug systematically - isolate and fix one issue at a time"
                "Read error messages carefully - they contain valuable clues"
                "Use version control to track changes and revert if needed"
                "Take breaks when stuck - fresh perspective often reveals solutions"
            )
            local selected_tip="${general_tips[RANDOM % ${#general_tips[@]}]}"
            echo -e "    ${FADED}Ancient wisdom: $selected_tip${NC}"
        fi
    fi

    # Check for historical milestones
    local historical_analysis=$(analyze_historical_trends)
    if [ -n "$historical_analysis" ]; then
        check_milestones "$historical_analysis"
    fi

    # Only show final message if no victory message was shown
    if [ "$victory_shown" = false ]; then
        local final_msg=$(get_random_message final_messages)
        echo -e "${BOLD_WHITE}$final_msg 🏛️${NC}"
    fi
}

echo ""
# Show motivational message with previous stats
show_motivational_message "$TOTAL_FILE_ERRORS" "$PASSED_INDIVIDUAL_TESTS" "$FAILED_INDIVIDUAL_TESTS" "$COMPILE_ERRORS" "$RUNTIME_ERRORS" "$EFFICIENCY_RATIO" "$TOTAL_TEST_TIME" "$CURRENT_CHALLENGE" "$PREV_ERRORS" "$PREV_PASSES" "$PREV_FAILURES" "$PREV_COMPILE_ERRORS" "$PREV_RUNTIME_ERRORS" "$PREV_EFFICIENCY_RATIO" "$PREV_PURE_TEST_TIME" "$PREV_IMPROVEMENT_STREAK" "$PREV_CURRENT_CHALLENGE" "$PREV_TIMESTAMP"

echo ""
# Archive status message with historical context
if [ -d "$SISYPHUS_DIR" ]; then
    archive_count=$(ls -1 "$SISYPHUS_DIR"/*.sp-stats 2>/dev/null | wc -l)
    first_run_date=""

    # Get date of oldest run
    oldest_file=$(ls -1 "$SISYPHUS_DIR"/*.sp-stats 2>/dev/null | tail -1)
    if [ -n "$oldest_file" ]; then
        first_run_date=$(grep "TIMESTAMP=" "$oldest_file" | cut -d'=' -f2 | cut -d' ' -f1)
    fi



    # Intelligent trend analysis - show only the most interesting pattern
    if [ -n "$archive_count" ] && [ "$archive_count" -ge 3 ]; then
        recent_files=($(ls -1 "$SISYPHUS_DIR"/*.sp-stats 2>/dev/null | tail -5))

        # Collect data points for pattern analysis
        declare -a total_issues_timeline=()
        declare -a runtime_errors_timeline=()
        declare -a passes_timeline=()
        declare -a efficiency_timeline=()

        for file in "${recent_files[@]}"; do
            compile_errors=$(grep "COMPILE_ERRORS=" "$file" | cut -d'=' -f2 | tr -d '\n\r' | tr -d ' ')
            runtime_errors=$(grep "RUNTIME_ERRORS=" "$file" | cut -d'=' -f2 | tr -d '\n\r' | tr -d ' ')
            failures=$(grep "FAILURES=" "$file" | cut -d'=' -f2 | tr -d '\n\r' | tr -d ' ')
            passes=$(grep "PASSES=" "$file" | cut -d'=' -f2 | tr -d '\n\r' | tr -d ' ')
            efficiency=$(grep "EFFICIENCY_RATIO=" "$file" | cut -d'=' -f2 | tr -d '\n\r' | tr -d ' ')

            if [[ "$compile_errors" =~ ^[0-9]+$ ]] && [[ "$runtime_errors" =~ ^[0-9]+$ ]] && [[ "$failures" =~ ^[0-9]+$ ]] && [[ "$passes" =~ ^[0-9]+$ ]]; then
                total_issues=$((compile_errors + runtime_errors + failures))
                total_issues_timeline+=("$total_issues")
                runtime_errors_timeline+=("$runtime_errors")
                passes_timeline+=("$passes")
                if [[ "$efficiency" =~ ^[0-9]+$ ]]; then
                    efficiency_timeline+=("$efficiency")
                fi
            fi
        done

        # Analyze patterns and show the most interesting one
        if [ ${#total_issues_timeline[@]} -ge 3 ]; then
            # Check for improvement streak (consecutive reductions from oldest to newest)
            streak_count=0
            for ((i=1; i<${#total_issues_timeline[@]}; i++)); do
                if [ "${total_issues_timeline[$i]}" -lt "${total_issues_timeline[$((i-1))]}" ]; then
                    streak_count=$((streak_count + 1))
                else
                    break
                fi
            done

            # Check for zero-error streak (from most recent backwards)
            zero_streak=0
            for ((i=${#total_issues_timeline[@]}-1; i>=0; i--)); do
                if [ "${total_issues_timeline[$i]}" -eq 0 ]; then
                    zero_streak=$((zero_streak + 1))
                else
                    break
                fi
            done

            # Check for consistent runtime beast slaying
            runtime_progress=0
            if [ ${#runtime_errors_timeline[@]} -ge 3 ]; then
                first_runtime="${runtime_errors_timeline[0]}"
                last_index=$((${#runtime_errors_timeline[@]} - 1))
                last_runtime="${runtime_errors_timeline[$last_index]}"
                if [ "$last_runtime" -lt "$first_runtime" ]; then
                    runtime_progress=$((first_runtime - last_runtime))
                fi
            fi

            # Check for test expansion (growing test suite)
            test_growth=0
            if [ ${#passes_timeline[@]} -ge 3 ]; then
                first_passes="${passes_timeline[0]}"
                last_passes_index=$((${#passes_timeline[@]} - 1))
                last_passes="${passes_timeline[$last_passes_index]}"
                if [ "$last_passes" -gt "$first_passes" ]; then
                    test_growth=$((last_passes - first_passes))
                fi
            fi



            # Show the most interesting trend (priority order)
            if [ "$zero_streak" -ge 2 ]; then
                echo -e "${GREEN}📈 Recent trend: 🏆 DIVINE PERFECTION - $zero_streak flawless runs in a row!${NC}"
            elif [ "$streak_count" -ge 1 ]; then
                echo -e "${GREEN}📈 Recent trend: 🚀 ASCENDING TRAJECTORY - $((streak_count + 1)) consecutive improvements!${NC}"
            elif [ "$runtime_progress" -ge 1 ]; then
                echo -e "${CYAN}📈 Recent trend: 🐲 HYDRA SLAYER - eliminated $runtime_progress segmentation faults & memory corruptions!${NC}"
            elif [ "$test_growth" -ge 3 ]; then
                echo -e "${YELLOW}📈 Recent trend: 🏗️ CODE ARCHITECT - expanded test suite by $test_growth tests!${NC}"
            else
                # Fallback to simple trend comparison
                first_total="${total_issues_timeline[0]}"
                last_total_index=$((${#total_issues_timeline[@]} - 1))
                last_total="${total_issues_timeline[$last_total_index]}"

                if [ "$first_total" -eq "$last_total" ]; then
                    echo -e "${PURPLE}📈 Recent trend: 🧘 STOIC CONSISTENCY - maintaining steady discipline!${NC}"
                elif [ "$last_total" -lt "$first_total" ]; then
                    echo -e "${CYAN}📈 Recent trend: 📉 GRADUAL MASTERY - slowly conquering challenges!${NC}"
                else
                    echo -e "${ORANGE}📈 Recent trend: ✨ PHOENIX RISING - transforming trials into wisdom!${NC}"
                fi
            fi
        fi
    fi
else
    echo -e "${CYAN}💡 Progress saved for next comparison${NC}"
fi

# Exit based on results
if [ "$COMPILE_ERRORS" -eq 0 ] && [ "$RUNTIME_ERRORS" -eq 0 ] && [ ${#FAILED_FILES[@]} -eq 0 ]; then
    echo -e "${GREEN}🎉 All tests passed without errors! The boulder stays at the top!${NC}"
else
    exit 1
fi
# --- The Sisyphus Cycle: A Five-Act Narrative ---
    # The player, as Sisyphus, will be shown one random line from the current act's array
    # each time they run the test suite, slowly revealing the story.

    # --- ACT I: THE NEW CURSE (LEVELS 1-10) ---
    # Theme: The setup. Introducing the characters, the oppressive setting,
    # and the soul-crushing reality of this new, intellectual punishment.
    declare -a act_one_greetings=(
        "🏛️ THE ARRIVAL|You are brought to a high tower in the palace of Knossos. A man with haunted eyes introduces himself as Daedalus. 'King Minos has a task for us,' he says. 'A glorious, eternal task.'"
        "🪨 THE FIRST COMMIT|Daedalus unrolls a vast, impossibly complex blueprint. 'Your curse has been... updated, Sisyphus. The gods grew bored of the boulder. Minos, however, has a use for your persistence.'"
        "👑 THE KING'S DECREE|King Minos addresses you from his throne, not looking at you. 'You will help my architect build a prison of logic and stone. It will be perfect. It will be endless. You will not fail.'"
        "⛓️ THE WEIGHT OF CODE|Your first bug. A simple off-by-one error. As you fix it, you feel a familiar weight settle on your shoulders. It is not stone, but the infinite mass of possibility."
        "🐂 A DISTANT ROAR|From somewhere deep below the foundations, a sound echoes—a roar of immense pain and rage. Daedalus flinches. 'Pay it no mind,' he says quickly. 'It is merely the... client.'"
        "🌅 THE FIRST SUNRISE|The Cretan sun reveals the dust motes dancing in the air. You did not sleep. You realize with a cold dread that in this new hell, fatigue is a luxury you are no longer afforded."
        "☕️ DAEDALUS'S WHISPER|The architect leans in, his breath smelling of stale wine. 'The King believes he is our master. He is wrong. We are slaves to this... this *idea*. The Labyrinth. It commands us now.'"
        "📜 THE UNREADABLE CONTRACT|A scroll is delivered, outlining your duties. The text shifts and writhes as you try to read it. The only clear clause is the last: 'The work will be complete when it is perfect. The work will never be complete.'"
        "🕊️ THE GHOST OF ATHENS|Daedalus stares east towards his homeland. 'They exiled me for one death,' he mutters. 'Here, they celebrate me for building a tomb that will consume thousands.'"
        "🤔 A FAMILIAR FEELING|You complete a complex function, and for a fleeting moment, you feel a sense of accomplishment. Then, the bug report arrives. The boulder has just rolled back to the bottom of the hill."
    )

    # --- ACT II: THE LABYRINTH'S NATURE (LEVELS 11-20) ---
    # Theme: Rising action. The mystery deepens. The Labyrinth is more than a building,
    # and the characters' true natures begin to surface.
    declare -a act_two_greetings=(
        "🌀 A SHIFTING BLUEPRINT|You glance at the master plan and swear a corridor you just memorized has changed its path. Daedalus dismisses it as a trick of the light, but you see him secretly make a new notation."
        "💧 THE WALLS WEEP|A strange, damp residue slicks the stones in a newly finished section. It smells of salt and sorrow. 'The stone of Crete is porous,' Daedalus explains, but he won't touch the walls."
        "🤫 THE SECRET LANGUAGE|Daedalus begins using a complex series of symbols only he understands. He calls it 'commenting his work.' You realize it is a second, hidden language embedded within the first."
        "👑 THE QUEEN'S VIGIL|You learn that Queen Pasiphaë provides the beast's food herself, leaving it at the Labyrinth's entrance each day. She has never seen the creature since its birth."
        "🐂 THE BEAST'S INTELLIGENCE|A section of wall is found battered from the inside, but not randomly. The impacts are focused on a single, load-bearing keystone. 'It's learning,' Daedalus whispers in horror."
        "🗣️ MINOS'S PARANOIA|'The tribute from Athens... it is a lie,' Minos rants during an inspection. 'They send not victims, but saboteurs! Their hero, Theseus, will try to steal my creation! Make it deadlier!'"
        "🧠 THE LOGIC'S ECHO|When you fix a particularly nasty bug, you can almost feel a ripple of... disappointment... emanate from the structure itself. The Labyrinth did not want that flaw removed."
        "🐦 A FEATHER ON THE FLOOR|You find a single, large feather near the forge—not from any bird you recognize. Daedalus snatches it and throws it into the flames, his face pale with a fear you do not yet understand."
        "🤝 THE SISYPHEAN COMPACT|'You are cursed to push, I am cursed to build,' Daedalus says, sharing his wine with you. 'Our fates are now intertwined. We will either perfect this place together or go mad in the attempt.'"
        "❓ A QUESTION OF PURPOSE|You ask Daedalus what happens when the Labyrinth is finished. He laughs, a dry, cracking sound. 'Finished? My boy, you still think this is about *building* something.'"
    )

    # --- ACT III: THE CRACKS APPEAR (LEVELS 21-30) ---
    # Theme: The midpoint. The characters begin to break under the strain.
    # The true, monstrous scale of the project becomes clear.
    declare -a act_three_greetings=(
        # The Revelation Begins
        "🎭 THE FIRST CRACK IN THE MASK|You catch Daedalus smiling - genuinely smiling - at a particularly elegant solution you've written. 'You know,' he says, 'Perdix would have liked that approach.' His face immediately darkens, but the moment of humanity lingers."
        "✨ SISYPHUS'S FIRST CHOICE|You discover a critical flaw that would let someone escape. Your hand hovers over the keyboard. For the first time in eternity, you have a choice. You document it in a comment: '// TODO: Never fix this.'"
        "🕊️ THE FEATHER'S SECRET|You find another feather, but this time you hide it before Daedalus sees. That night, you study it by candlelight. It's perfectly engineered - each barb calculated for maximum lift. Someone is planning to fly."
        "💝 AN UNEXPECTED ALLY|A young servant girl leaves bread and wine at your workstation. 'My sister was in last year's tribute,' she whispers. 'Whatever kindness you can build into those walls...' She doesn't finish. She doesn't need to."
        "🗝️ THE HIDDEN PATTERN|You realize Daedalus has been embedding a pattern in his 'errors' - they're not mistakes, they're messages. When decoded, they spell out: 'THE MONSTER IS NOT THE ONLY PRISONER HERE.'"
        "🌟 SISYPHUS THE TEACHER|A young apprentice is assigned to help you. You find yourself explaining not just the code, but the beauty in the logic. For a moment, you remember: you were once more than just a curse. You were a king who loved to build."
        "🎪 THE GAME WITHIN THE GAME|You start leaving your own messages in the code. Small jokes. Elegant solutions that serve no purpose but beauty. If you must push this boulder, you'll choose HOW you push it."
        "🔮 THE ORACLE'S VISIT|A priestess arrives, claiming to need to bless the structure. She looks directly at you: 'The gods are watching, Sisyphus. But perhaps... they're not the only ones who can change the rules of the game.'"
        "🦅 DAEDALUS'S CONFESSION|After too much wine, Daedalus admits: 'I've built two labyrinths, Sisyphus. One of stone that holds the beast. One of guilt that holds me. But you... you're building a third one. What does yours contain?'"
        "💪 THE BOULDER BECOMES LIGHTER|Something strange happens. As you fix a particularly complex bug, you feel... joy? The boulder is still there, but for the first time, you're not just pushing it. You're sculpting it."
    )

    # --- ACT IV: INESCAPABLE TRUTHS (LEVELS 31-40) ---
    # Theme: The climax. The major secrets are revealed, confronting the characters with the full horror of their situation.
    declare -a act_four_greetings=(
        # The Alliance Forms
        "🤝 THE ARCHITECT'S PROPOSITION|Daedalus pulls you aside, his eyes clear for the first time in months. 'I know what you've been doing with the code. The hidden paths. Help me save my son, and I'll help you save your soul.'"
        "🗺️ THE TRUE LABYRINTH|'The Labyrinth isn't the building,' Daedalus reveals, showing you the REAL blueprint. 'It's the entire palace. The entire kingdom. Every person trapped in their role. But we're going to build an exit - for everyone.'"
        "✍️ CODE NAMES|You and Daedalus develop a secret language within the code. Variables named after hope. Functions that sound like prayers. Every bug fix now carries a hidden purpose: creating a backdoor to freedom."
        "🎭 THE PERFORMANCE|You play your roles perfectly by day - the cursed king and the mad architect. But by night, you're revolutionaries, encoding escape routes in error messages, hiding wings in exception handlers."
        "👑 MINOS'S BLINDNESS|The King inspects your work, praising its perfection. He cannot see that every 'perfect' function contains a flaw that only you and Daedalus know. Pride has made him blind. You exchange the briefest glance with Daedalus - a smile."
        "🌅 THE COUNTDOWN BEGINS|'Three more moons,' Daedalus whispers. 'When the spring tribute arrives, we fly. But first, we must ensure the Labyrinth can be solved. No more children will die in our creation.'"
        "💡 SISYPHUS'S INNOVATION|You realize something Daedalus hasn't: the curse can be turned into a gift. Your eternal persistence means you can test every possible path. You begin mapping EVERY route through the maze, creating the ultimate walkthrough."
        "🎪 THE SECRET THREAD|You embed a algorithm in the Labyrinth's core: anyone who enters with love in their heart will find the walls themselves guiding them to safety. Ariadne's thread will work because you're making sure it will."
        "🔥 THE FORGING OF HOPE|Together, you and Daedalus work on the wings. But these aren't just wings of wax and feathers. They're wings of code, of logic, of perfectly calculated rebellion. Each feather is a function. Each function is a prayer."
        "⚡ THE MOMENT OF TRUTH|'Tomorrow, we test them,' Daedalus says, holding up the wings. 'If they fail, we remain prisoners. If they succeed...' He looks at you. 'Friend, if they succeed, even the gods will learn that mortals can rewrite their fates.'"
    )

    # --- ACT V: THE ETERNAL CYCLE (LEVELS 41-50) ---
    # Theme: The resolution. The acceptance of the absurd. The Labyrinth is "done," but the work never ends.
    declare -a act_five_greetings=(
        # The Philosophical Climax
        "🌅 THE MORNING OF ESCAPE|Dawn breaks. The wings are ready. Daedalus straps them on his son. But as you prepare your own set, you realize something: you're not sure you want to leave. This curse, this code, it's become... yours."
        "🕊️ ICARUS SOARS|You watch the boy fly, laughing with pure joy. For a moment, all the pain, all the labor, all the endless debugging - it was worth it to see this one soul truly free. Then the sun grows hot..."
        "💔 THE FALL AND THE CHOICE|Icarus falls. Daedalus screams. But you... you're already at the terminal, coding. If you can't save the boy, you can save the next one. And the next. And the next. Your curse has become your purpose."
        "🎭 THE KING'S OFFER|Minos finds you at your terminal. 'Daedalus has fled. You could follow. I won't stop you.' He pauses. 'Or you could stay. Maintain the Labyrinth. Save those who enter. Your choice, Sisyphus.' But you've already chosen."
        "✨ THE REVELATION|As you code, you understand: The gods didn't curse you to push a boulder. They gave you the only gift that matters - eternal purpose. Every bug is a life saved. Every fix is a small rebellion against fate."
        "🏛️ THE NEW MYTHOLOGY|You embed a message in the Labyrinth's core: 'HERE WORKED SISYPHUS, WHO CHOSE HIS STONE.' Future heroes will find it and understand: sometimes the greatest freedom is choosing your own chains."
        "🌊 THE ETERNAL DEBUGGER|A new error appears. You smile. Tomorrow there will be more. Forever there will be more. But each one is a chance to save someone, to improve something, to create meaning from meaninglessness. You begin to type."
        "💪 THE HAPPY SISYPHUS|You push back from your terminal and laugh - a real, deep laugh. The gods wanted to break you with eternal labor. Instead, you've become unbreakable. The boulder rolls down. You crack your knuckles. 'Again,' you say, and mean it."
        "🎨 THE MASTERPIECE NEVER ENDS|New tributes arrive. You've hidden seventeen different escape routes in the code. Theseus will find his thread because you put it there. Heroes succeed because Sisyphus ensures it. Your curse is their salvation."
        "♾️ ONE MUST IMAGINE SISYPHUS CODING|The sun sets. It rises. The bugs appear. You fix them. But now you know the secret - in the space between the bug and the fix, in that moment of problem-solving, you are completely, perfectly, eternally free. The cursor blinks. You smile. Time to push the boulder up the hill again. And you wouldn't have it any other way."
    )
