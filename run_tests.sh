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
        "run-test-c-parser-basic")
            echo "tests/linter/test_c_parser_basic.c"
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

# --- THE SISYPHUS NARRATIVE: A FIVE-ACT STORY ---
# Each level-up reveals one line from the story, building the narrative over time

# --- ACT I: THE NEW CURSE (LEVELS 1-10) ---
# Theme: The setup. Introducing the characters, the oppressive setting,
# and the soul-crushing reality of this new, intellectual punishment.
declare -a act_one_story=(
    "🏛️ THE ARRIVAL|You are brought to a high tower in the palace of Knossos. A man with haunted eyes introduces himself as Daedalus. 'King Minos has a task for us,' he says. 'A glorious, eternal task.'"
    "🪨 THE FIRST COMMIT|Daedalus unrolls a vast, impossibly complex blueprint. 'Your curse has been... updated, Sisyphus. The gods grew bored of the boulder. Minos, however, has a use for your persistence.'"
    "👑 THE KING'S DECREE|King Minos addresses you from his throne, not looking at you. 'You will help my architect build a prison of logic and stone. It will be perfect. It will be endless. You will not fail.'"
    "⛓️ THE WEIGHT OF CODE|Your first bug. A simple off-by-one error. As you fix it, you feel a familiar weight settle on your shoulders. It is not stone, but the infinite mass of possibility."
    "🐂 A DISTANT ROAR|From somewhere deep below the foundations, a sound echoes—a roar of immense pain and rage. Daedalus flinches. 'Pay it no mind,' he says quickly. 'It is merely the... client.'"
    "👤 THE QUEEN'S SHADOW|A figure in royal robes watches from a distant balcony as you work. When you look up, she vanishes. Daedalus notices your glance. 'Queen Pasiphaë,' he whispers. 'She comes to watch... but never to speak.'"
    "🔒 THE SEALED CHAMBER|Daedalus shows you the blueprint's center—a room marked with symbols you don't recognize. 'This chamber,' he says, his voice tight, 'must never be accessible from the outside. Only from within. The King's specific orders.'"
    "⚔️ THE CAPTAIN'S INSPECTION|A scarred guard captain arrives to review your progress. His eyes linger on your hands as you code. 'I've executed architects before,' he says casually. 'The King values... precision.' Daedalus's hand trembles as he rolls up the blueprints."
    "📐 THE IMPOSSIBLE SPECIFICATION|Daedalus hands you a new requirement scroll. The mathematics make no sense—corridors that loop back on themselves, doors that open both ways simultaneously. 'Make it work,' he says desperately. 'The King tested the logic himself.'"
    "🌙 THE NIGHT SHIFT|As darkness falls, Daedalus lights a single candle and continues working. 'The real work begins now,' he murmurs. 'When the palace sleeps, we build the sections the King must never see.' He slides you a second, hidden blueprint."
)

# --- ACT II: THE LABYRINTH'S NATURE (LEVELS 11-20) ---
# Theme: Rising action. The mystery deepens. The Labyrinth is more than a building,
# and the characters' true natures begin to surface.
declare -a act_two_story=(
    "🏰 MEMORIES OF CORINTH|You catch yourself designing an elegant archway—the same style you once commissioned for your palace. 'You were a king,' Daedalus observes quietly. 'I can see it in how you structure the logic. You built for beauty once. Now we build for horror.'"
    "🍷 THE ARCHITECT'S TREMOR|Daedalus's hands shake as he draws. Not from wine this time—from something else. He keeps glancing at a locked drawer in his desk. 'Some prisons,' he mutters, 'hold more than monsters. Some hold the reasons we deserve to be here.'"
    "💔 THE WEIGHT OF BETRAYAL|'I know what you did,' Daedalus says suddenly over his cups. 'Cheated death, betrayed the gods' hospitality. But at least your crime was against immortals. Mine...' He stops, staring at his hands. 'Mine was against someone who trusted me.'"
    "🪶 A CHILD'S DRAWING|While organizing blueprints, a small parchment falls out—a child's drawing of a bird-man flying over the sea. Daedalus snatches it away, but not before you see the inscription: 'For Papa, when we fly together - I.' His eyes are wild with panic."
    "👑 SISYPHUS'S CONFESSION|'I used to watch my workers build,' you tell Daedalus, surprising yourself. 'Every stone placed was a choice, a small rebellion against entropy. Now I build prisons. The gods' joke—turning creation into damnation.'"
    "🔨 THE NEPHEW'S SHADOW|Daedalus flinches when you use a particularly clever technique. 'Perdix used to do that,' he whispers. 'My nephew. He was brilliant. Too brilliant.' His hand unconsciously moves to his chest, as if feeling for an old wound."
    "🐂 THE BEAST GROWS RESTLESS|The roars from below have changed—no longer just rage, but something worse: rhythm. Pattern. 'It's counting,' Daedalus realizes with horror. 'Three roars, pause. Seven roars, pause. It's learning mathematics. Your code is teaching it to think.'"
    "⚖️ THE BUILDER'S BURDEN|'We're alike, you and I,' Daedalus says, watching you debug with inhuman persistence. 'Both punished for our cleverness. Both building our own hells. The difference is...' He pauses, touching that locked drawer again. 'I still have something left to lose.'"
    "🗿 THE LIVING STONE|You place your hand on a completed wall and feel a pulse—not mechanical, but organic. 'It's learning from us,' you realize aloud. 'Our guilt, our regret, our...' Daedalus finishes: 'Our desperate need to protect what we love. Yes. The Labyrinth knows.'"
    "🌅 A PROMISE IN CODE|Late at night, Daedalus shows you a function he's written—beautiful, elegant, purposeless. 'For after,' he says cryptically. 'When the spring winds come. When young birds test their wings.' He deletes it immediately, but you memorize every line."
)

# --- ACT III: THE CRACKS APPEAR (LEVELS 21-30) ---
# Theme: The midpoint. The characters begin to break under the strain.
# The true, monstrous scale of the project becomes clear.
declare -a act_three_story=(
    "🎭 THE MASK SLIPS|Daedalus hasn't slept in three days. He's muttering to blueprints, arguing with walls. 'They're wrong!' he screams suddenly. 'The calculations! Someone changed them!' You check—he's right. The Labyrinth is rewriting itself."
    "⚡ THE FIRST REBELLION|You deliberately introduce a bug—a way out, disguised as a memory leak. Your cursor hovers over the delete key. For the first time in eternity, you choose defiance. The comment reads: '// Some errors are features.'"
    "🔥 THE BEAST BREAKS FREE|Screams echo through the palace. The Minotaur has breached the inner walls. Guards flee as stone and bronze buckle under inhuman strength. 'It's too strong!' Daedalus cries. 'The rage is evolving faster than our prison!'"
    "🌙 A MOTHER'S LULLABY|The rampage stops abruptly. Through the chaos, you hear it—a woman's voice, singing softly. The beast's roars fade to whimpers. Guards whisper of the Queen walking alone into the ruins, emerging unharmed. 'Even monsters,' Daedalus says quietly, 'remember their mothers.'"
    "🪶 THE WORKSHOP BETRAYAL|You find a half-built wing frame hidden in Daedalus's quarters. But it's child-sized. 'You're building escape for someone,' you confront him. His eyes fill with tears. 'The King has my son. Everything I build... it's all for Icarus.'"
    "🗡️ THE CAPTAIN'S SUSPICION|The guard captain corners you alone. 'Architects who whisper at night often whisper their last,' he warns. 'Whatever conspiracy you're weaving into those walls... I'm watching.' His hand rests on his sword."
    "💀 SISYPHUS BREAKS|You realize with horror: you've been happy. Solving problems, creating elegant code—you've forgotten this is a tomb. You punch the wall, bloodying your knuckles. 'I'm not their pet architect!' But you return to coding. You always return."
    "📜 THE KING'S NEW DEMAND|Minos arrives unannounced, Theseus's name on his lips like poison. 'Change everything,' he commands. 'Make it crueler. If heroes are coming, let them find not a maze, but a meat grinder.' Daedalus's stylus snaps in his grip."
    "🌊 THE POINT OF NO RETURN|'We could stop,' Daedalus whispers one night. 'Refuse. Let them kill us.' You both know it's a lie. The work has consumed you. The Labyrinth isn't just your prison—it's your child. And like all parents, you're damned to see it complete."
    "🔮 THE ORACLE'S WARNING|A priestess arrives with a prophecy: 'Two shall enter the workshop as builders. Only one shall leave as a man. The other...' She looks at you with pity. 'The other shall leave as something else entirely.'"
)

# --- ACT IV: INESCAPABLE TRUTHS (LEVELS 31-40) ---
# Theme: The climax. The major secrets are revealed, confronting the characters with the full horror of their situation.
declare -a act_four_story=(
    "🤝 THE DESPERATE PROPOSITION|Daedalus grabs your arm, his grip iron-strong with desperation. 'I know about your hidden exits. You know about my son. Help me get Icarus out, and I'll ensure your code saves every tribute who enters. We can beat them all—Minos, the gods, fate itself.'"
    "🤔 SISYPHUS'S DOUBT|You study Daedalus in the candlelight. This man murdered his nephew. His 'accidents' destroy lives. Can you trust someone whose guilt runs deeper than yours? But then you remember: even damned souls can choose redemption."
    "💭 THE WEIGHT OF DECISION|Three days you deliberate. Daedalus grows frantic, leaving blueprints of wings on your desk. 'We fly over the labyrinth,' he whispers. 'Land on the tower where they keep him. The guards won't expect an assault from above.' It's madness. It might work."
    "✊ THE PACT IS SEALED|'I'll help you,' you finally say. Daedalus weeps—the first honest tears you've seen. 'But we do this my way. Every function we write saves a life. Every bug we leave helps a hero. If we're going to break the rules, we break them all.'"
    "🪶 STEALING DIVINITY|You begin collecting materials. Feathers from the sacred birds. Wax from the temple candles. Bronze gears from the Labyrinth's mechanisms. 'We're not just building wings,' Daedalus explains. 'We're stealing the gods' own tools of flight.'"
    "🔧 THE SECRET WORKSHOP|Behind a false wall, you've built a second laboratory. Here, physics bends to desperate will. The wings take shape—part engineering, part prayer. 'Each feather must be perfect,' Daedalus insists. 'One flaw and Icarus falls.'"
    "📐 SISYPHUS'S MATHEMATICS|Your curse becomes crucial. You calculate wind patterns obsessively, test weight distributions endlessly. 'I can push this boulder forever,' you realize. 'I can perfect these wings through infinite iteration.' Daedalus watches in awe as you refine the impossible."
    "🌙 THE GUARD ROTATIONS|You've mapped every sentry's pattern. The tribute ships arrive in seven days—chaos will reign. 'During the ceremony,' Daedalus plans, 'all eyes on the maze entrance. We launch from the north tower. Fifteen minutes to reach Icarus.'"
    "🎭 THE FINAL DECEPTION|By day, you embed more cruelties into the Labyrinth, pleasing Minos. By night, you encode escape routes so subtle even Daedalus misses some. 'You're playing a deeper game,' he observes. You nod. 'Someone needs to stay behind. Maintain the system. That someone is immortal.'"
    "⚡ TOMORROW WE FLY|The wings are complete. Tested in secret midnight flights from the tower. Daedalus trembles with hope and terror. 'Tomorrow, when the tributes arrive, we strike. Icarus will be free.' He pauses. 'Sisyphus... what if the sun—' 'Then we fly at dawn,' you interrupt. 'Before it rises too high.'"
)

# --- ACT V: THE ETERNAL CYCLE (LEVELS 41-50) ---
# Theme: The resolution. The acceptance of the absurd. The Labyrinth is "done," but the work never ends.
declare -a act_five_story=(
    "🌅 THE DAWN OF FREEDOM|The tribute ships arrive in chaos. Guards rush to the harbor. You and Daedalus climb the north tower, Icarus between you, wings strapped tight. 'Remember,' Daedalus tells his son, 'not too high.' The boy nods, eyes bright with dreams of sky."
    "🕊️ THE IMPOSSIBLE FLIGHT|They leap. By all laws of gods and men, they should plummet. But they rise. Icarus laughs—a sound of pure, defiant joy. For one perfect moment, you've beaten them all. The boy soars higher, drunk on freedom, reaching for the sun..."
    "💔 THE FALL AND THE SILENCE|Icarus falls. Daedalus screams. The sound tears the morning apart—a father's soul breaking. Then... nothing. No splash. No crash. Just absence. When you look again, the sky is empty. Both of them—gone. You stand alone on the tower."
    "🏛️ THE KING'S KNOWING SMILE|You return to find Minos at your terminal, calm as stone. 'They flew,' he says. It's not a question. 'The father escaped. The boy... who can say? The sea keeps its secrets.' He gestures to your keyboard. 'But you remain. Interesting.'"
    "⚖️ THE DEVIL'S BARGAIN|'Here's my offer,' Minos says. 'Maintain the Labyrinth. Make it kinder. Save who you can. In return, I won't hunt Daedalus.' He smiles coldly. 'Refuse, and I'll make the maze a slaughterhouse. Your choice, eternal one.'"
    "✨ THE REVELATION|As your fingers find the keys, you understand: you were never meant to escape. Daedalus built wings. You built purpose. The gods didn't curse you—they prepared you. For this. For choosing to stay when you could have fled."
    "📝 THE CODER'S MANIFESTO|You write a new function: compassion.exe. Then another: hope.dll. The Labyrinth becomes your canvas. Each algorithm is a small kindness, each bug fix a life saved. You're not pushing a boulder. You're sculpting salvation."
    "🌊 THE MYTH REWRITES ITSELF|Years pass. Tales spread of the Labyrinth that helps heroes, the maze that teaches rather than kills. They whisper of a ghost in the code—Sisyphus, who chose his chains and transformed them into wings for others."
    "💪 THE HAPPY SISYPHUS|You push back from your terminal and laugh—a real, deep laugh. The gods wanted to break you with eternal labor. Instead, you've become unbreakable. The boulder rolls down. You crack your knuckles. 'Again,' you say, and mean it."
    "♾️ ONE MUST IMAGINE SISYPHUS CODING|The sun sets. It rises. The bugs appear. You fix them. But now you know the secret—in the space between the bug and the fix, in that moment of problem-solving, you are completely, perfectly, eternally free. The cursor blinks. You smile. Time to push the boulder up the hill again. And you wouldn't have it any other way."
)

# Function to get story line based on level
get_story_line() {
    local level=$1
    local story_line=""
    local act_name=""

    if [ $level -le 10 ] && [ $level -ge 1 ]; then
        story_line="${act_one_story[$((level - 1))]}"
        act_name="Act I: The New Curse"
    elif [ $level -le 20 ] && [ $level -ge 11 ]; then
        story_line="${act_two_story[$((level - 11))]}"
        act_name="Act II: The Labyrinth's Nature"
    elif [ $level -le 30 ] && [ $level -ge 21 ]; then
        story_line="${act_three_story[$((level - 21))]}"
        act_name="Act III: The Cracks Appear"
    elif [ $level -le 40 ] && [ $level -ge 31 ]; then
        story_line="${act_four_story[$((level - 31))]}"
        act_name="Act IV: Inescapable Truths"
    elif [ $level -le 50 ] && [ $level -ge 41 ]; then
        story_line="${act_five_story[$((level - 41))]}"
        act_name="Act V: The Eternal Cycle"
    fi

    echo "$story_line|$act_name"
}

# Function to display level-up story
display_level_up_story() {
    local new_level=$1
    local story_data=$(get_story_line $new_level)
    IFS='|' read -r story_line act_name <<< "$story_data"

    if [ -n "$story_line" ]; then
        IFS='|' read -r title content <<< "$story_line"
        echo ""
        echo -e "${PURPLE}🎊 CONGRATULATIONS! You've reached Level $new_level!${NC}"
        echo -e "${BOLD_WHITE}$title${NC}"
        echo -e "    $content"
        echo -e "    ${FADED}This story fragment has been added to sisyphus.legend${NC}"
        echo ""
    fi
}

# Function to save story progress to legend file
save_story_to_legend() {
    local level=$1
    local story_data=$(get_story_line $level)
    IFS='|' read -r story_line act_name <<< "$story_data"

    if [ -n "$story_line" ]; then
        # Create or append to story section in sisyphus.legend
        local story_section_exists=$(grep -c "# SISYPHUS STORY PROGRESS" "$SISYPHUS_LEGEND" 2>/dev/null || echo "0")

        if [ "$story_section_exists" -eq 0 ]; then
            # Add story section to legend file
            echo "" >> "$SISYPHUS_LEGEND"
            echo "# SISYPHUS STORY PROGRESS" >> "$SISYPHUS_LEGEND"
            echo "# Each level reveals a fragment of your eternal tale" >> "$SISYPHUS_LEGEND"
        fi

        # Check if this story line already exists
        IFS='|' read -r title content <<< "$story_line"
        local story_exists=$(grep -c "^STORY_LEVEL_${level}=" "$SISYPHUS_LEGEND" 2>/dev/null || echo "0")

        if [ "$story_exists" -eq 0 ]; then
            echo "STORY_LEVEL_${level}=${act_name}|${title}|${content}" >> "$SISYPHUS_LEGEND"
        fi
    fi
}

echo -e "${PURPLE}🚀 Daedalus Test Suite Runner${NC}"
echo -e "${CYAN}Enhanced with Sisyphus Progress Archive & Historical Analysis${NC}"
echo -e "${GRAY}Run './run_tests.sh help' for available commands${NC}"
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
        local total_project_xp=
        local project_level=0

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
        echo "0|0|0|0|0|0|0|0|$(date '+%Y-%m-%d')|0|0"
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
    # Recalculate project level based on total XP using correct formula
    local correct_project_level=0
    local cumulative_xp=0
    while true; do
        local next_level=$((correct_project_level + 1))
        local xp_needed=$(( 600 + (next_level * next_level * next_level * 98) + (next_level * next_level * 296) + (next_level * 294) ))
        if [ $((cumulative_xp + xp_needed)) -gt $total_project_xp ]; then
            break
        fi
        cumulative_xp=$((cumulative_xp + xp_needed))
        correct_project_level=$((correct_project_level + 1))
    done

    # Store level-up info for later display (don't display during stats update)
    LEVELS_GAINED=""
    echo -e "${GRAY}[LEVEL DEBUG] Starting level: $project_level, Total XP: $total_project_xp, Calculated level: $correct_project_level, Session XP: $session_xp${NC}" >&2
    if [ "$correct_project_level" -gt "$project_level" ]; then
        # Save all level-up info to display later
        for ((level=$((project_level + 1)); level<=correct_project_level; level++)); do
            LEVELS_GAINED="${LEVELS_GAINED}${level},"
        done
        echo -e "${GRAY}[LEVEL DEBUG] Level up detected! LEVELS_GAINED: $LEVELS_GAINED${NC}" >&2
    else
        echo -e "${GRAY}[LEVEL DEBUG] No level up detected${NC}" >&2
    fi

    # Read existing story sections if they exist
    local story_sections=""
    if [ -f "$SISYPHUS_LEGEND" ] && grep -q "# SISYPHUS STORY PROGRESS" "$SISYPHUS_LEGEND" 2>/dev/null; then
        # Extract everything after the stats section (including the story header)
        story_sections=$(awk '/# SISYPHUS STORY PROGRESS/{found=1} found{print}' "$SISYPHUS_LEGEND")
    fi

    # Write updated legend stats to temporary file to avoid data loss
    cat > "$SISYPHUS_LEGEND.tmp" << EOF
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

    # Append story sections back if they existed
    if [ -n "$story_sections" ]; then
        echo "" >> "$SISYPHUS_LEGEND.tmp"
        echo "$story_sections" >> "$SISYPHUS_LEGEND.tmp"
    fi

    # Atomically move temporary file to final location
    mv "$SISYPHUS_LEGEND.tmp" "$SISYPHUS_LEGEND"

    # Now save any new story progress for levels gained this run
    if [ -n "$LEVELS_GAINED" ]; then
        # Remove trailing comma and split into array
        local levels_gained_temp="${LEVELS_GAINED%,}"
        IFS=',' read -ra LEVEL_ARRAY_TEMP <<< "$levels_gained_temp"

        for level in "${LEVEL_ARRAY_TEMP[@]}"; do
            save_story_to_legend "$level"
        done
    fi
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

# Level-up tracking
LEVELS_GAINED=""

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


# Function to extract test counts from output
extract_test_counts() {
    local output="$1"

    # Look for the FINAL Sisyphus test summary line: "🧪 Maze Sections Built: 6 | 🏗️ Completed: 4 | 🪨 Needs Rework: 2"
    # Use tail to get the last occurrence in case there are multiple summary lines
    local total=$(echo "$output" | grep -o "Maze Sections Built: [0-9]*" | tail -1 | grep -o "[0-9]*" || echo "0")
    local passed=$(echo "$output" | grep -o "Completed: [0-9]*" | tail -1 | grep -o "[0-9]*" || echo "0")
    local failed=$(echo "$output" | grep -o "Needs Rework: [0-9]*" | tail -1 | grep -o "[0-9]*" || echo "0")

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
run_test "Widgets Basic" "run-test-widgets-basic"
run_test "Widgets Advanced" "run-test-widgets-advanced"
run_test "Initialize Basic" "run-test-initialize-basic"
run_test "Initialize Edge Cases" "run-test-initialize-edge"
run_test "Initialize Simple" "run-test-init-simple"
run_test "Text Basic" "run-test-text-basic"
run_test "Text Edge Cases" "run-test-text-edge"


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



# Final Summary
SUCCESSFUL_FILES=$((PASSED_FILES))
FAILED_FILE_COUNT=$((${#FAILED_FILES[@]}))

echo ""
echo -e "${PURPLE}📊 Current Test Results Summary${NC}"
# Read current project level and XP for progress bar
legend_line="$(read_legend_stats)"
IFS='|' read -r total_runs total_quests hydra_beasts blueprint_flaws oracle_mysteries divine_streaks longest_streak perfect_ascensions first_quest total_project_xp project_level <<< "$legend_line"

# Calculate XP progress within current level using C header formula
# XP formula: 600 + (level^3 * 98) + (level^2 * 296) + (level * 294)
get_xp_for_level() {
    local level=$1
    echo $(( 600 + (level * level * level * 98) + (level * level * 296) + (level * 294) ))
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
next_level=$((actual_project_level + 1))
xp_needed_for_next=$(get_xp_for_level $next_level)

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
    echo -e "${BOLD_WHITE}🚀 Code Efficiency: Your Code is ${GREEN}${EFFICIENCY_RATIO}x${BOLD_WHITE} faster than the Sisyphus Test Engine!${NC}"
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

        # --- The Rival Architect: Daedalus on Archimedes ---
        "⚙️ A TOYMAKER'S WIDGETS|'Look at this 'aWidget_t',' Daedalus scoffs, reviewing a plan for a control panel. 'Buttons and sliders... Archimedes builds toys for children. We are designing a cage for a concept.'"
        "🎨 THE SIMPLICITY OF COLOR|Daedalus grinds a new pigment. 'Archimedes defines his 'aColor_t' with four simple bytes. He does not understand the infinite shades of sorrow and dread required for these walls.'"
        "🔊 THE FOLLY OF SOUND|'I've seen the plans for his audio engine,' Daedalus sneers. 'An 'a_PlaySoundEffect' to delight the masses. Here, the only sound that matters is the silence when a tribute's screams have faded.'"

        # --- The Nature of the Code ---
        "🏛️ ARCHITECTURE OF THE DAMNED|'The 'Daedalus' library is well-named,' you think, looking at a chain of pointers. 'Each structure is a marvel of logic, and each is a link in an inescapable chain.'"
        "❓ A SELF-AWARE ERROR|You trigger a 'fatal' error log. The message printed is not from your code. It simply says, 'I know you can see me.' You erase the log file and do not speak of it."
        "📜 THE INFINITE SCROLL|Daedalus holds up a 'dString_t' in memory. 'It can grow forever, Sisyphus. Just like the King's list of requirements. A perfect tool for an endless task.'"

        # --- Dawn Breeze Damnations ---
        "🌅 THE SCENT OF THE SEA|A cool breeze blows in from the Aegean, carrying the scent of salt and freedom. It only serves to make the stale air of the workshop feel more suffocating."
        "🌄 A PAINTER'S SKY|The dawn paints the sky in hues of rose and gold, a spectacle of divine beauty. You see it only as a reflection in the cold, unfeeling bronze of the Labyrinth's half-finished gates."
        "🕊️ THE BIRDS' MOCKING SONG|You hear birdsong from the olive groves outside. They sing of a world that is free, a world that wakes and lives. Here, in the tower, the only sound is the scratching of your stylus."
        # --- Personal Philosophy: Moral Rebellion vs. Absurdity ---
        "🌅 DAWN'S DEFIANCE|The sunrise illuminates a choice: build the maze as designed, or secretly build it to help future heroes. In this moment of clarity, rebellion feels more real than the stone itself."
        "⚡ MORNING'S REBELLION|'The gods gave me eternal labor,' you think, watching the dawn, 'but they cannot control the *manner* of my work. Every kindness I code is theft from their cruel design.'"
        "🔥 FIRST LIGHT'S TRUTH|As dawn breaks, you realize the absurdity isn't in the endless work—it's in pretending you have no choice about how to do it. Today, you choose compassion over compliance."

        # --- Creating Heroes ---
        "🌱 HERO SEED PLANTING|With each dawn, you hide another tiny flaw in the maze's logic—breadcrumbs for future heroes. You may never escape, but others will, and that makes you their silent architect."
        "🗝️ KEY FORGING DAWN|The morning light reveals your true purpose: not to build a perfect prison, but to forge the tools future champions will need. Every bug you 'accidentally' leave is a gift to tomorrow's courage."
        "🕯️ TORCHBEARER'S PROMISE|As the sun rises, you make a sacred vow: I may be trapped in this workshop, but my code will light the way for heroes I'll never meet. My chains become their liberation tools."

        # --- Melancholy Optimism ---
        "🌸 BITTER BLOSSOMS|The morning air carries the scent of Cretan wildflowers—beautiful life growing despite the shadow of the labyrinth. Even in places built for death, beauty insists on existing."
        "🌊 GENTLE PERSISTENCE|Like the dawn that returns despite every dark night, hope has a way of surviving even in the depths of Daedalus's workshop. Today feels different, somehow lighter."
        "⭐ DISTANT KINDNESS|Somewhere beyond this maze, someone will benefit from the work you do today. That unknown gratitude makes the weight of the stylus feel less like a burden."

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

        # --- The Test of Sisyphus ---
        "⚖️ A SINGLE ASSERTION|'Each 'TEST_ASSERT is a law of nature we impose upon the chaos,' Daedalus explains. 'When it fails, it means reality itself has rejected our decree.'"
        "⚔️ A NEW BOUT IN THE ARENA|Daedalus points to a new file. 'Time to 'RUN_TEST', Sisyphus. Another corridor to validate, another beast to face. Let us see if your logic holds this time.'"
        "📜 THE FINAL TALLY|'The 'TEST_SUITE_END' is not a victory, but a judgment,' Daedalus says, looking at the final results. 'It merely tells us the weight of the boulder we must push tomorrow.'"

        # --- Wisdom of the Damned ---
        "🤔 THE NATURE OF THE BOULDER|You realize the boulder was a mercy. Its purpose was simple: to be pushed. The purpose of this code is to be *perfect*, a goal that feels infinitely further than the summit ever did."
        "☕️ CLARITY'S CRUELTY|'The morning light is a curse,' Daedalus mutters into his cup. 'It shows you not what you have accomplished, but only how far you have yet to go. It illuminates the true scale of the prison.'"
        "✨ A SPARK OF DEFIANCE|You fix a bug, and for a moment, you feel a spark of pride. This is not the gods' punishment. This is your work. A subtle but profound act of defiance in the face of eternity."

        # --- The Architect's Tools ---
        "🔗 THE CHAIN OF MEMORY|Daedalus points to a 'dLinkedList_t'. 'See? Each flaw is linked to the next, an unbreakable chain of consequence. Our job is not to break the chain, but to master it.'"
        "🗺️ MAPPING THE CELL|'We use a 'dVec3_t' not to map a world, but to define the precise coordinates of a cage,' Daedalus says, his voice flat. 'X, Y, and Z. The three dimensions of our shared tomb.'"
        "📦 A BOX OF PAIN|'The King has sent another 'dArray_t' of requirements,' Daedalus sighs, unrolling a scroll. 'A dynamic array of impossible tasks. It grows larger every day.'"
        
        # --- Personal Philosophy: Moral Rebellion vs. Absurdity ---
        "⚔️ MORAL COMBAT|'The King commands efficiency,' you think, deliberately coding a slower but more merciful path through the maze. 'But I command my conscience. This is where I make my stand.'"
        "🎭 ABSURD HEROISM|You realize the cosmic joke: the gods meant to break you with meaningless work, but you've found the ultimate meaning—choosing kindness when cruelty would be easier."
        "🔥 REBELLION'S FUEL|Each test you run is a small act of defiance. They wanted you to build death; you build hope disguised as death. The absurdity becomes your weapon."

        # --- Creating Heroes ---
        "🛡️ SHIELD CRAFTING|While Daedalus designs death traps, you secretly encode mercy protocols. Every function becomes a shield for future champions, forged in the fire of your own imprisonment."
        "🗺️ MAP MAKING|Your 'errors' in the code are actually a secret map for heroes. You're not debugging—you're leaving a trail of breadcrumbs through the labyrinth of logic."
        "⚔️ WEAPON SMITH|They think you're building a prison. You know you're forging the exact weaknesses future heroes will need. Every line of code is a blade waiting for the right hand."

        # --- Melancholy Optimism ---
        "📚 LIVING LIBRARY|Your accumulated knowledge will outlive this workshop. Somewhere, someday, a programmer will benefit from the patterns you're discovering in this cursed code."
        "🌱 UNDERGROUND GROWTH|Even in the artificial light of the workshop, you feel something growing inside you—not despair, but a strange, determined peace with the importance of your work."
        "🎨 ACCIDENTAL BEAUTY|The algorithms you're forced to write have an unexpected elegance. Even tools of imprisonment can achieve something beautiful when crafted with care."

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

        # --- The MIDAS Toolchain: Reflections on the Tools ---
        "💡 A LACK OF METIS|'I have cunning, Sisyphus, but no wisdom,' Daedalus laments, staring at a flawed logical proof. 'Metis has forsaken this place. There is no counsel here, only complexity.'"
        "💰 THE CURSE OF MIDAS|A guard mentions the immense cost of the Labyrinth. 'The King turns the entire treasury to stone,' he laughs. Daedalus hears him and mutters, 'A Midas touch indeed. Everything he values becomes his prison.'"
        "🏛️ ARCHIMEDES'S EASE|'I hear Archimedes draws his engines of war in the sand,' Daedalus scoffs. 'He does not know the terror of building something that thinks, that watches, that *waits*.'"

        # --- The Daedalus Code: Meta-Narrative ---
        "🌀 THE KINEMATIC BEAST|'We do not define the beast's 'dKinematicBody',' Daedalus explains, gesturing wildly at the blueprints. 'We merely define the walls. Its rage and sorrow are the 'force' and 'velocity'. We only channel them.'"
        "📜 THE BROKEN STRING|A messenger scroll arrives torn in half. 'An omen,' Daedalus says, pushing the pieces together. 'The King's words have been split. We must treat this as a 'd_StringSplit' and hope the delimiter is not our own necks.'"
        "🔢 THE MATRIX OF FATE|'This is not a room, it's a 'dMat4x4_t'!' Daedalus exclaims, pointing at a corridor's design. 'Change one value in the matrix, and the entire reality of the space transforms. It is a terrifying power.'"

        # --- The Ixion Cycle: Hubris and Repetition ---
        "🔥 THE FIERY WHEEL|You are stuck in a recursive function with no base case. A true infinite loop. You feel the heat of Ixion's punishment and understand that some cycles are not meant to be broken."
        "👑 THE HUBRIS OF THE ARCHITECT|'I once believed I was better than the gods,' Daedalus confesses, his voice low. 'Ixion was punished for desiring a god. My crime... was believing I *was* one. This Labyrinth is my wheel.'"
        "🔄 A FAMILIAR FAILURE|A bug you just fixed reappears with the exact same error code. It's the third time today. You are no longer debugging. You are simply spinning on the King's cruel, fiery wheel."
    
        # --- Personal Philosophy: Moral Rebellion vs. Absurdity ---
        "🌋 MOLTEN DEFIANCE|The afternoon heat reminds you of Tartarus, but now you choose your own punishment: building mercy into a machine designed for murder. Free will burns hotter than any forge."
        "⚡ THUNDEROUS CHOICE|In the oppressive heat, you make the most radical choice possible: caring about strangers you'll never meet. This is how mortals steal fire from the gods."
        "🔥 FURNACE FREEDOM|The workshop burns like a forge, but you realize you're not being refined—you're doing the refining. Every choice to help rather than harm is pure, distilled freedom."

        # --- Creating Heroes ---
        "🌉 BRIDGE TO TOMORROW|The afternoon pressure crystallizes your purpose: build bridges others can cross to safety. Your suffering becomes the foundation for their success."
        "💎 HERO FORGE|This oppressive heat is the perfect temperature for forging champions. Every difficulty you code into the maze is precisely calibrated to create rather than destroy heroes."
        "⚔️ DESTINY SHAPER|You're not just building a labyrinth—you're creating the exact conditions that will transform frightened tributes into legendary heroes. Destiny requires adversity."

        # --- Melancholy Optimism ---
        "🍯 SWEETENED SUFFERING|Even this brutal afternoon labor has moments of unexpected satisfaction. There's honey hidden in the most bitter work when you know it serves a greater purpose."
        "🌾 HARVEST WISDOM|Every painful lesson learned in this heat becomes grain that will feed future programmers. Your suffering is not waste—it's investment in others' success."
        "🕯️ Steady Light|In the chaos of afternoon demands, you maintain a steady inner flame. Your consistency becomes a beacon others will follow long after this workshop crumbles."

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

        # --- The Rival Architect: Daedalus on Archimedes ---
        "🗣️ 'CHILDISH CIRCLES'|'I hear that fool Archimedes spends his days drawing circles in the sand,' Daedalus scoffs. 'Let him 'a_DrawCircle' for children. We are building a prison for a god. Our geometry must be divine, and terrible.'"
        "🗣️ 'A SUPERFICIAL GLAZE'|Daedalus inspects a perfectly rendered image on a test display. ''a_Blit'... it's a parlor trick. Anyone can copy an image. It takes a true genius to build the thing that *casts* the shadow.'"
        "🗣️ 'THE NOISE OF SYRACUSE'|'They say Archimedes can make bronze sing with his 'a_InitAudio' nonsense,' Daedalus says, his voice dripping with contempt. 'The only sound of true creation is the silence of a problem solved.'"

        # --- The Price of Knowledge ---
        "🔑 THE KEYMAKER'S FATE|'The locksmith who forged the core mechanism was executed this evening,' Daedalus says, not looking up. 'The King has decided that only the architects may know the true design.'"
        "🗣️ THE CAPTAIN'S REPORT|The captain of the Royal Guard appears at dusk. 'A message from the King,' he says, his voice flat. 'The beast grows stronger. The Labyrinth must grow faster.'"
        "🤔 SISYPHUS'S GAMBIT|You realize a bug you created is the only thing preventing a major section from becoming active. To fix it is to complete it. To leave it is to risk the architect's wrath. What is the greater sin?"

        # --- The Labyrinth's Deepening Mystery ---
        "🌬️ THE BREATH OF THE MAZE|As the evening air cools, a faint, rhythmic draft comes from the completed corridors—in, then out. It's as if the Labyrinth itself has begun to breathe in the dark."
        "🤔 THE BOULDER'S MERCY|You realize that pushing the boulder was a mercy. It was a simple task with a simple failure. This... this Labyrinth... it fails in ways that question your very sanity, your ability to reason. It's a punishment for the soul."
        "🗣️ 'IT LEARNS MY TRICKS'|'I built a false loop, a simple trick to confound a simple mind,' Daedalus says, his voice tight with fear. 'But the beast no longer falls for it. It learns, Sisyphus. It adapts to my own thoughts.'"
    
        # --- Personal Philosophy: Moral Rebellion vs. Absurdity ---
        "🌙 LUNAR REBELLION|As twilight falls, you code by candlelight and conscience. The darkness hides your defiance: every merciful subroutine is a victory over the absurd cruelty of kings."
        "⭐ STELLAR DEFIANCE|The evening stars witness your quiet revolution: choosing to build hope into a framework of despair. This is how mortals rewrite the stories gods try to tell."
        "🌆 DUSK'S VICTORY|In the fading light, you see clearly—the ultimate absurdity isn't the work itself, but accepting that you have no choice in how to do it. Tonight, you choose compassion."

        # --- Creating Heroes ---
        "🌟 STAR FORGE|As night falls, you realize you're not building a prison—you're forging stars. Each hero you secretly enable will shine light into someone else's darkness."
        "🧭 NAVIGATION GIFT|Your evening work focuses on subtle guidance systems—ways for future heroes to find their path through the maze. You're not just a prisoner; you're a lighthouse keeper."
        "💫 LEGEND WEAVER|In the twilight hours, you weave the foundation of legends. Years from now, heroes will triumph using tools you're crafting in this lonely workshop."

        # --- Melancholy Optimism ---
        "🌈 EVENING RAINBOW|The day's harsh light softens into something beautiful. Even the most difficult work can end with moments of unexpected grace and quiet satisfaction."
        "🕊️ PEACEFUL PURPOSE|As evening settles, you feel a strange peace. Your work matters, even if you'll never see its fruits. That unknown impact makes the loneliness bearable."
        "📚 TWILIGHT WISDOM|The fading light brings clarity: your suffering has meaning because you choose to make it meaningful. That choice transforms everything."

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
        "🐂 THE BEAST IN THE DARK|In the profound silence of midnight, you can almost hear the Minotaur's slow, heavy breathing. It is not sleeping. It is waiting. The code must be perfect to hold it."

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
        "👻 DAEDALUS WARNS|Daedalus mutters from his workbench, not looking up from his scrolls: 'The foundation stones are always the first to crumble. Test what should never break—because that is precisely what will shatter when the Minotaur grows restless.'"

        # --- The Prisoner's Perspective ---
        "🪨 THE ECHOING BOULDER|In the dead silence, you can almost hear the scrape of a great boulder on a stone path. You realize with horror that the sound is only in your head, a memory of a simpler torment."
        "🤝 A SHARED DAMNATION|You press your ear to the floor and hear a faint, rhythmic tapping from below. One-two. One-two. The Minotaur is not raging. It is counting the seconds, just as you are."
        "🎭 PERFECTION FOR NO ONE|A chilling thought: you are building a perfect prison that no one will ever see, to contain a creature whose only crime was being born. Your eternal labor serves only to hide a King's shame."

        # --- The MIDAS Stack: Reflections on the Tools ---
        "🧠 THE TITAN'S WISDOM|'This code... it has a cunning of its own now,' Daedalus mutters, pointing at a recursive function. 'It would take the wisdom of Metis herself to untangle this thread. And I am no Titan.'"
        "🔥 THE FIERY WHEEL OF LOGIC|You are stuck in a loop, fixing a bug that creates another. You think of Ixion, bound to his fiery wheel. You realize his punishment was simpler; at least his wheel was physical."
        "📐 A FLAWED GEOMETRY|'Archimedes draws circles on sand and calls it genius,' Daedalus scoffs into his wine. 'He has never tried to map a corridor that changes based on who is walking it.'"

        # --- The Nature of the Code ---
        "📜 THE SCROLL OF SOULS|You see the list of tributes and realize it's just a 'dArray_t', an array of names. To the gods, to Minos, these lives are just data points in a larger, crueler function."
        "💬 A CRY FOR HELP|A 'd_Log' entry appears in the system console, originating from a file you haven't touched in weeks. It prints a single, cryptic word: 'HELP'. There is no other trace of its source."
        "🔗 THE STRINGS OF FATE|Daedalus holds up a length of frayed rope. 'I can build a perfect 'dString_t' that will never break,' he says, 'but I cannot fix the tangled threads of fate that brought us to this room.'"

        # --- Existential Dread and the Curse ---
        "🎭 THE PURPOSE OF PAIN|You fix a bug, and for a moment, the Minotaur's low growl from below softens into something almost like gratitude. A horrifying thought: perhaps your work is not a curse, but a kindness."
        "♾️ THE ETERNAL REFACTOR|Daedalus laughs quietly. 'Sisyphus, do you understand yet? Minos does not want a Labyrinth. He wants the *building* of a Labyrinth. The project is the prison, not the product.'"
        "👤 THE CARETAKER'S KNOWLEDGE|Staring at the silent code, you know with absolute certainty that Daedalus will leave. He will escape or die. But you... you will remain. Someone must maintain the system. The curse has found its true host."

        # --- Personal Philosophy: Moral Rebellion vs. Absurdity ---
        "🌑 MIDNIGHT REVOLUTION|In the deep darkness, your most radical act: caring about the welfare of tributes you'll never meet. This is how you steal meaning from meaninglessness."
        "⭐ COSMIC DEFIANCE|The stars watch as you commit the ultimate rebellion—building love into a machine designed for hate. The universe itself witnesses your refusal to surrender your humanity."
        "🔥 ETERNAL FLAME|In the profound midnight silence, you tend the fire of moral choice. They can imprison your body, enslave your labor, but they cannot control the kindness you code into their cruel designs."

        # --- Creating Heroes ---
        "🌟 MIDNIGHT STAR BIRTH|In the deepest darkness, you give birth to future legends. Every line of merciful code becomes the star by which heroes will navigate their darkest hours."
        "🗝️ KEY TO TOMORROW|While the palace sleeps, you forge the keys future champions will need. Your insomnia becomes their liberation. Your sleepless nights become their bright futures."
        "💫 HERO CONSTELLATION|Working by candlelight, you connect the dots between present suffering and future triumph. Each hero you enable becomes part of a constellation of hope."

        # --- Melancholy Optimism ---
        "🌊 OCEAN OF TIME|In the midnight stillness, you understand—your work will ripple forward like waves, touching shores you'll never see. That distant impact makes the solitude beautiful."
        "⭐ ETERNAL VIGIL|The stars keep you company in your lonely work. Like them, you shine steadily in the darkness, a fixed point of light for others to navigate by."
        "🔥 PHOENIX PREPARATION|In these deep night hours, you're not just coding—you're preparing your own resurrection. Every hero you help becomes part of your own immortal story."

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

        "🧠 METIS AWAKENS WITH THE SEASON|A scroll arrives from the Oracle, bearing the seal of Metis. 'The Titan of Wisdom has seen your code in her dreams,' it reads. 'She offers counsel: refactor before the tribute ships arrive.'"
        "⚡ IXION'S SPRING TORMENT|Daedalus points to a spinning mechanism in the courtyard—a prototype wheel that turns endlessly. 'Ixion's punishment inspires my designs,' he says. 'Infinite loops have their place, but not in our memory allocations.'"
        "📐 ARCHIMEDES IN THE GARDEN|A messenger brings news from Syracuse: 'The great geometer has calculated the perfect trajectory for spring projectiles.' Daedalus scoffs: 'While he draws circles in sand, we build circles in logic.'"
        "🌿 THE RENEWAL OF SISYPHUS|Spring's arrival reminds you that even eternal curses have seasons. Each new bloom outside mirrors a new test suite within. The boulder may be code now, but it still requires the same endless pushing."
        "🔧 DAEDALUS'S SPRING CLEANING|'The winter's work has left our data structures cluttered,' Daedalus announces, holding up scrolls covered in crossed-out functions. 'Like Metis cleaning Athena's wisdom chamber, we must organize our thoughts.'"
        "🌊 THE MIDAS CONVERGENCE|Five separate scrolls arrive simultaneously—reports from across the Greek world. Metis speaks of wisdom, Ixion of restraint, Archimedes of precision, and Daedalus of craft. All await your response, Sisyphus. The toolchain of legends converges in spring."
        
        # --- Personal Philosophy: Moral Rebellion vs. Absurdity ---
        "🌸 SPRING'S REBELLION|The flowers bloom in defiance of the maze's shadow. Today you choose to code like them—beautiful, stubborn, refusing to accept that death is the only destiny."
        "🌱 RENEWAL'S REVOLT|Spring whispers that even eternal curses can be reborn with new purpose. You plant seeds of mercy in code designed for murder—your most radical act of springtime rebellion."
        "🦋 METAMORPHOSIS CHOICE|The butterflies emerging from their cocoons inspire a revelation: transformation is always possible. Today you transform from victim to secret architect of heroes."

        # --- Creating Heroes ---
        "🌸 HERO GARDEN|Like a gardener planting seeds for future harvests, you code hidden strengths into the maze. Spring teaches that heroes, like flowers, need the right conditions to bloom."
        "🌱 CHAMPION CULTIVATION|The tribute ships bring frightened youth, but your secret modifications will transform them into legends. Spring is the season of potential heroes taking root."
        "🦋 LEGEND CHRYSALIS|Spring's transformations remind you that heroes aren't born—they're made. Every merciful subroutine you hide becomes a cocoon from which champions will emerge."

        # --- Melancholy Optimism ---
        "🌸 BITTER BLOSSOMS|The palace gardens bloom with heartbreaking beauty above a labyrinth of death. Even in places of planned tragedy, life insists on creating something beautiful."
        "🌱 HOPEFUL SHOOTS|Tiny green shoots push through cracks in the workshop stones. If life can find a way through solid rock, perhaps hope can find a way through this curse."
        "🦋 DELICATE PROMISES|Each butterfly that visits the window carries the same message: beautiful things can emerge from the darkest transformations. Your work might yet yield unexpected grace."

    )

    declare -a summer_greetings=(
        # --- Theme: Oppressive Heat, Inescapable Fate ---
        "🔥 THE FORGE OF HELL|The summer sun turns the workshop into an oven. Daedalus claims the heat reminds him of Hephaestus's forge, but his wild eyes tell you it reminds him more of the fires of Tartarus."
        "🔥 MINOS'S BURNING GAZE|The King spends the long, hot days watching your progress from his shaded balcony. His impatience grows with the heat, his demands becoming more erratic and impossible."
        "🐍 A PLAGUE OF ERRORS|The oppressive heat seems to spawn bugs like insects in a swamp. Every line of code you fix, two more seem to appear in its place, as if the Labyrinth itself is feverish."
        "🌊 THE SALT-STAINED BLUEPRINTS|The sea air is thick and heavy. The papyrus scrolls curl with dampness, blurring the lines of the master plan. Daedalus accuses the sea god himself of trying to sabotage his work."
        "🤔 A FAMILIAR BURN|The heat of the forge where the Labyrinth's iron is shaped feels familiar. You recall the flames of the underworld and wonder which punishment is worse: a simple, honest fire, or this sun-baked intellectual torment."
        "🗣️ A GUARD'S CYNICISM|A guard, bringing you water, mutters, 'The King hides from the sun while we toil in it. He fears the light will reveal the true monster is not the one in the maze.'"

        "🌡️ IXION'S FIERY WHEEL TURNS FASTER|The heat makes everything worse—especially infinite loops. 'Ixion's wheel spins faster in summer,' Daedalus observes, watching his performance profiler scroll endless warnings. 'The heat reveals which code burns too bright.'"
        "🧠 METIS SEEKS SHADE|Even wisdom wilts in the Cretan summer. A message from the Oracle: 'Metis counsels: refactor during the cool morning hours. The blazing sun corrupts both judgment and memory allocation.'"
        "⚙️ ARCHIMEDES' COOLING CALCULATIONS|From Syracuse comes a treatise on heat dissipation in mechanical systems. 'Archimedes calculates the optimal fan blade angles,' Daedalus reads aloud. 'If only he could calculate a way to cool our burning CPUs.'"
        "🔥 THE SUMMER OF INFINITE COMPILATION|The bronze in the forges melts faster, but your compilation times grow longer. Summer reveals the true cost of complex systems—every nested function layer adds heat, every recursive call risks the Ixion spiral."
        "🌵 DAEDALUS'S DESERT WISDOM|'In the desert, only the essential survives,' Daedalus says, pruning unnecessary functions from the core library. 'Summer teaches us what Metis knows: true wisdom lies in what you choose NOT to build.'"
        "☀️ THE MIDAS TRIALS BY FIRE|The summer solstice brings messages from all corners of the Mediterranean. Each hero of the toolchain reports the same: only the most optimized, the most essential, the most perfect code survives the season of testing. Your labyrinth must be forged in summer's fire to survive winter's trials."
    
        # --- Personal Philosophy: Moral Rebellion vs. Absurdity ---
        "🔥 FORGE OF FREEDOM|The summer heat burns away pretense. In this furnace, you realize the ultimate truth: they can enslave your body and time, but your choices remain molten and malleable."
        "☀️ SOLAR DEFIANCE|Under the blazing sun, you code with deliberate compassion. Each merciful algorithm is solar-powered rebellion—drawing energy from the very light that oppresses you."
        "🌋 VOLCANIC CHOICE|The workshop becomes a volcano of moral decision. Every function you write erupts with either cruelty or kindness. You choose to make this heat forge heroes instead of victims."

        # --- Creating Heroes ---
        "🔥 CHAMPION FORGE|The summer heat creates perfect forging conditions. You hammer courage into code, tempering future heroes in the flames of your own suffering."
        "☀️ SOLAR HEROICS|Like the sun that gives life to the world, your hidden modifications will give strength to tomorrow's champions. Summer teaches that even harsh conditions can nurture greatness."
        "🌋 LEGEND FOUNDRY|In the volcanic heat of the workshop, you pour molten hope into hero-shaped molds. The tributes will enter as sacrifice; your work ensures they leave as legends."

        # --- Melancholy Optimism ---
        "🔥 BEAUTIFUL BURNING|Even the oppressive heat has its own harsh beauty. Your sweat becomes holy water, blessing code that will save lives you'll never meet."
        "☀️ PERSISTENT LIGHT|The summer sun never gives up, returning each day despite the night's darkness. Your work has the same relentless quality—always pushing toward illumination."
        "🌋 CREATIVE DESTRUCTION|Like volcanic eruptions that eventually create fertile ground, your current suffering is preparing the soil where future heroes will grow to greatness."

    )

    declare -a autumn_greetings=(
        # --- Theme: Decay and the Inevitable Harvest ---
        "🍂 THE HARVEST OF TRIBUTES|As the world outside gathers its crops, Crete prepares for its own grim harvest. The Labyrinth's gates are oiled, the tribute chambers are cleaned. The machine awaits its purpose."
        "🌬️ A CHILLING WIND|The first autumn wind howls through the tower, scattering scrolls. For a moment, it sounds like a scream from the maze below. Daedalus doesn't even flinch anymore."
        "🥀 THE QUEEN'S GARDEN WILTS|Queen Pasiphaë no longer walks the gardens. The flowers have died, and she remains in her chambers. Her sorrow has consumed the season, a blight upon the land."
        "🦉 THE OWL'S JUDGMENT|An owl, Athena's sacred bird, lands on the windowsill each evening. Daedalus, once her favored craftsman, refuses to look at it, as if terrified of the wisdom—or condemnation—it might offer."
        "🤔 A HARVEST OF NOTHING|You watch the farmers bringing in their bounty. You have labored just as they have, yet your only harvest is a more perfect prison, a more complete damnation. There is no crop to gather from your work."
        "🗣️ THE HERO'S ARRIVAL|'A ship from Athens,' the guard captain reports to Daedalus. 'It carries a volunteer. A prince named Theseus. He says he has come to slay the beast.' Daedalus only smiles, a thin, cruel line on his lips."

        "🍇 METIS'S HARVEST WISDOM|The Oracle sends the season's final counsel: 'Metis observes that autumn teaches the greatest lesson—what appears to be decay is actually preparation. Your deprecated functions are not death, but transformation.'"
        "🎯 ARCHIMEDES' BALLISTIC CALCULATIONS|News arrives from the siege of Syracuse: 'Archimedes has perfected his war engines for autumn campaigns.' Daedalus studies the reports: 'While he calculates trajectories of death, we calculate pathways of entrapment.'"
        "⛓️ IXION'S WHEEL SLOWS WITH THE SEASON|The cooler air brings relief to overheating systems. 'Even Ixion's eternal punishment yields to autumn's mercy,' Daedalus notes, watching the performance monitors stabilize. 'Our memory leaks seal themselves in the crisp air.'"
        "📚 THE GREAT DEBUGGING HARVEST|Autumn brings the season's final push to eliminate all remaining bugs before winter's freeze. Like farmers gathering the last grain, you and Daedalus methodically harvest every remaining flaw from the code."
        "🏛️ DAEDALUS'S ARCHITECTURAL REVIEW|'The summer's construction must now be tested by autumn's storms,' Daedalus declares, reviewing the season's work. 'Only the structures that survive the first cold winds deserve to stand through winter.'"
        "🌾 THE MIDAS COLLECTIVE GATHERS|As autumn deepens, reports flood in from across the Greek world. Metis has gathered wisdom, Ixion has tracked performance through the season's trials, Archimedes has refined his calculations, and Daedalus has built foundations that will endure. Now the harvest of knowledge converges in your hands, Sisyphus. The toolchain's true power reveals itself: not individual excellence, but collective mastery."
    
        # --- Personal Philosophy: Moral Rebellion vs. Absurdity ---
        "🍂 HARVEST REBELLION|As farmers gather crops, you harvest the courage to defy your purpose. Each line of merciful code is wheat stolen from the King's cruel granary."
        "🌬️ WIND OF CHANGE|The autumn wind carries away the last pretenses. You strip away the illusion that you have no choice and embrace the terrifying freedom of moral decision."
        "🥀 BEAUTIFUL DECAY|The dying leaves teach profound truth: even endings can be gorgeous when chosen consciously. Your rebellion against the maze's purpose is autumn-beautiful."

        # --- Creating Heroes ---
        "🍂 LEGEND SEEDS|Like maple trees releasing countless seeds on autumn winds, your code scatters hope for future heroes across the labyrinth's deadly passages."
        "🌬️ CHAMPION WINDS|The autumn gusts carry your secret modifications throughout the maze. Soon, heroes will breathe in the very air you've filled with hidden advantages."
        "🥀 HEROIC PREPARATION|As nature prepares for winter's trials, you prepare the tributes for their trials. Your hidden gifts will be the difference between death and legend."

        # --- Melancholy Optimism ---
        "🍂 GOLDEN MEMORIES|The autumn leaves turn gold before falling, reminding you that even temporary beauty has infinite value. Your work will be brief but golden in its impact."
        "🌬️ CARRYING FORWARD|Like seeds on the wind, your influence will travel far beyond this workshop. Autumn teaches that endings are often beginnings in disguise."
        "🥀 GRACEFUL SACRIFICE|The trees sacrifice their leaves to survive winter. Your sacrifice of personal freedom ensures others will survive their trials. There's grace in chosen loss."

    )

    declare -a winter_greetings=(
        # --- Theme: Isolation, Cold Clarity, and Existential Dread ---
        "❄️ THE SILENCE OF THE TOMB|The first snow blankets the palace in silence. The Labyrinth below feels less like a prison and more like a mausoleum. You are not building a cage; you are perfecting a tomb for a living god."
        "🥶 THE POINT OF NO RETURN|Trapped by winter storms, Daedalus stares at the master blueprint for days without speaking. At last, he whispers, 'Sisyphus... I have designed it so perfectly that I no longer know the way out myself.'"
        "🕯️ THE ETERNAL NIGHT|The nights are longest now. You have more hours in the dark with the code than in the light. You feel more at home in the logical maze of the blueprints than in the waking world."
        "🧊 A FROZEN HEART|King Minos's demands cease. He no longer watches from his balcony. In the cold of winter, even his rage seems to have frozen, replaced by a cold, calculating acceptance of the monster he keeps."
        "🤔 A COLD COMFORT|The biting winter wind feels like a relief. It is a clean, honest pain, unlike the twisted, complex agony of a recursive function with no exit condition."
        "🗣️ DAEDALUS'S CHILLING PROPHECY|'The Labyrinth will be finished when the winter ends, Sisyphus,' Daedalus says, his breath misting in the cold air. 'And when it is... the King has no more use for its architect. Or his assistant.'"

        "🧊 METIS IN WINTER'S GRIP|The final scroll from the Oracle arrives, sealed with ice: 'Metis speaks from the depths of winter wisdom: In the season of death, only the most essential truths survive. Strip your code to its bare bones.'"
        "❄️ IXION'S WHEEL FROZEN IN TIME|The performance monitors show strange readings—perfect optimization, zero waste, crystalline efficiency. 'Even Ixion's eternal punishment yields to winter's absoluteness,' Daedalus whispers. 'His wheel has frozen at the moment of perfect balance.'"
        "🏹 ARCHIMEDES BESIEGED|Dark news from Syracuse: 'The Romans besiege the city. Archimedes' geometric brilliance is the only thing standing between civilization and barbarity.' Daedalus stares at the message. 'We are all besieged, my friend. All defending impossible positions.'"
        "🗿 THE WINTER OF FINAL ASSEMBLY|In winter's clarity, the entire system comes together. Each component of the MIDAS toolchain finds its place: Metis provides the wisdom, Ixion ensures efficiency, Archimedes calculates precision, Daedalus builds the foundation. And you, Sisyphus, you provide the eternal will to push it all forward."
        "🌨️ DAEDALUS'S WINTER REVELATION|'I understand now,' Daedalus says, watching snow fall on the maze below. 'We were never building a prison for the Minotaur. We were building a system—a perfect system that would operate without us. The labyrinth is becoming its own entity.'"
        "❄️ THE FROZEN TOOLCHAIN|Winter brings a terrible clarity: the MIDAS toolchain is complete, but completion means obsolescence. Metis no longer needs to counsel—the code is wise. Ixion no longer needs to watch—the system is perfectly efficient. Archimedes no longer needs to calculate—the geometry is absolute. Daedalus no longer needs to build—the foundation is eternal. But you, Sisyphus... you still need to push. Because someone must maintain perfection. Someone must keep the eternal system running. Winter reveals your true purpose: not to finish the task, but to never let it stop."

        # --- Personal Philosophy: Moral Rebellion vs. Absurdity ---
        "❄️ CRYSTALLINE REBELLION|Winter's clarity strips away all excuses. In the cold truth of this season, you see clearly: every moment of kindness coded into cruelty is a victory over the absurd."
        "🧊 FROZEN DEFIANCE|Like ice that expands and cracks stone, your moral choices have power to shatter even the most carefully constructed systems of oppression."
        "🌨️ PURE RESISTANCE|Snow covers the labyrinth in innocent white, reminding you that purity can blanket even the most corrupt foundations. Your choices remain unstained by their purposes."

        # --- Creating Heroes ---
        "❄️ WINTER STARS|In the longest nights, you code by starlight, creating navigation points for heroes lost in future darkness. Your work becomes their guiding constellation."
        "🧊 PRESERVED HOPE|Winter preserves what matters most. Your hidden modifications freeze into the maze's core, waiting for spring heroes to discover what you've preserved for them."
        "🌨️ SILENT GIFTS|Like snow that falls silently but transforms everything, your secret kindnesses will change the maze in ways no one will trace back to their gentle creator."

        # --- Melancholy Optimism ---
        "❄️ DIAMOND TEARS|Winter tears freeze into diamonds of clarity. Your suffering crystallizes into wisdom that will sparkle in future heroes' victories."
        "🧊 ETERNAL PRESERVATION|Like ancient seeds preserved in ice, your good intentions will survive long past this workshop's memory, sprouting in times and places you cannot imagine."
        "🌨️ QUIET BEAUTY|Snow makes even the labyrinth beautiful. Your presence makes even this cursed work beautiful. Sometimes the most profound changes happen in perfect silence."

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
    local next_level=$((actual_project_level + 1))
    local xp_needed_for_next=$(get_xp_for_level $next_level)

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
            echo -e "${PURPLE}🏛️  THE FOUNDATION IS LAID! $total_curr_issues minor flaws await the master's touch${NC}"
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
            echo -e "${GREEN}⚡THE LABYRINTH YIELDS! One more passage cleared, one more mystery solved${NC}"
            echo -e "    ${FADED}'In the endless work, Sisyphus, each small victory is a rebellion against fate'${NC}"
        fi

        # Story-driven streak tracking with existential weight
        if [ "$current_streak" -ge 5 ]; then
            echo -e "${PURPLE}🌀 THE ETERNAL RHYTHM! For $current_streak cycles, the boulder has rolled only upward${NC}"
            echo -e "    ${FADED}Daedalus whispers: 'You have found the secret, Sisyphus. Embrace the absurd, and it becomes divine.'${NC}"
        elif [ "$current_streak" -ge 3 ]; then
            echo -e "${CYAN}⛓️  BREAKING THE PATTERN! $current_streak successive triumphs over the Labyrinth's chaos${NC}"
            echo -e "    ${FADED}The chains of your curse grow lighter with each victory. Purpose emerges from punishment.${NC}"
        elif [ "$current_streak" -ge 2 ]; then
            echo -e "${YELLOW}🪨 THE BOULDER LISTENS! $current_streak pushes without it rolling back down${NC}"
            echo -e "    ${FADED}You begin to understand: the stone is not your enemy, but your teacher.${NC}"
        fi

        # Narrative-driven technical achievements that advance the story
        if [ "$compile_change" -gt 0 ]; then
            echo -e "${CYAN}🏗️  DAEDALUS'S BLUEPRINTS PERFECTED! $compile_change architectural flaws corrected${NC}"
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
                "${GREEN}🏛️  THE ETERNAL MOMENT! In this instant, Sisyphus, the boulder rests at the summit${NC}|'Impossible,' whispers Daedalus. 'We have achieved what the gods deemed eternal punishment.'"
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
                "${CYAN}🏗️  DAEDALUS INSPECTS! 'Only $total_curr_issues final touches remain on my masterpiece'${NC}|Master architect nods approvingly - the labyrinth nears completion"
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
        "🏗️  'Scrolls are read by many, but written by few' - Daedalus's architectural clarity!"
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
            "${GREEN}🏛️  DAEDALUS ADMIRES THE PERFECTION! 'The blueprints are flawless. The structure is absolute. I could not have built it better myself.'${NC}|The master craftsman inspects your work and finds no fault. The Labyrinth is a mathematical marvel.|${BOLD_WHITE}✨ ARCHITECT'S BLESSING! Your logic has surpassed the creator's vision!${NC}"
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
# Display any level-up messages that occurred during this run
if [ -n "$LEVELS_GAINED" ]; then
    # Remove trailing comma and split into array
    LEVELS_GAINED="${LEVELS_GAINED%,}"
    IFS=',' read -ra LEVEL_ARRAY <<< "$LEVELS_GAINED"

    for level in "${LEVEL_ARRAY[@]}"; do
        display_level_up_story "$level"
    done
fi

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

# Function to view story progress from legend file
view_sisyphus_story() {
    if [ ! -f "$SISYPHUS_LEGEND" ]; then
        echo -e "${RED}🔴 No Sisyphus legend found. Begin your journey to unlock the story.${NC}"
        return 1
    fi

    echo -e "${PURPLE}📜 THE ETERNAL TALE OF SISYPHUS${NC}"
    echo -e "${CYAN}Your story unfolds with each level...${NC}"
    echo ""

    local current_act=""
    local story_found=false

    # Read story entries from legend file
    for level in {1..50}; do
        local story_entry=$(grep "^STORY_LEVEL_${level}=" "$SISYPHUS_LEGEND" 2>/dev/null || echo "")

        if [ -n "$story_entry" ]; then
            story_found=true
            # Extract story components
            story_entry="${story_entry#STORY_LEVEL_${level}=}"
            IFS='|' read -r act_name title content <<< "$story_entry"

            # Display act header if it changed
            if [ "$act_name" != "$current_act" ]; then
                current_act="$act_name"
                echo ""
                echo -e "${BOLD_WHITE}=== $current_act ===${NC}"
                echo ""
            fi

            # Display story line
            echo -e "${YELLOW}Level $level:${NC} ${BOLD_WHITE}$title${NC}"
            echo -e "         $content"
        fi
    done

    if [ "$story_found" = false ]; then
        echo -e "${YELLOW}No story fragments unlocked yet. Continue your journey to reveal the tale.${NC}"
    fi

    # Show current progress
    local legend_line="$(read_legend_stats)"
    IFS='|' read -r total_runs total_quests hydra_beasts blueprint_flaws oracle_mysteries divine_streaks longest_streak perfect_ascensions first_quest total_project_xp project_level <<< "$legend_line"

    echo ""
    echo -e "${CYAN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    echo -e "${CYAN}Current Level: ${BOLD_WHITE}$project_level${NC} / 50"

    # Calculate XP progress for next level
    local next_level=$((project_level + 1))
    local xp_needed_for_next=$(( 600 + (next_level * next_level * next_level * 98) + (next_level * next_level * 296) + (next_level * 294) ))
    local xp_progress=$(get_xp_in_current_level $total_project_xp $project_level)

    echo -e "${CYAN}Progress to Level $next_level: ${BOLD_WHITE}$(format_number_with_commas $xp_progress) / $(format_number_with_commas $xp_needed_for_next)${NC} XP"

    if [ "$project_level" -lt 50 ]; then
        echo ""
        echo -e "${GRAY}Continue pushing the boulder to unlock Level $next_level's story fragment...${NC}"
    else
        echo ""
        echo -e "${GREEN}✨ You have discovered the complete tale of Sisyphus!${NC}"
    fi
}

# Handle CLI arguments for viewing story
if [ "$1" = "story" ] || [ "$1" = "--story" ]; then
    view_sisyphus_story
    exit 0
fi

# Handle help command
if [ "$1" = "help" ] || [ "$1" = "--help" ] || [ "$1" = "-h" ]; then
    echo -e "${PURPLE}🏛️ SISYPHUS TEST RUNNER - COMMAND HELP${NC}"
    echo ""
    echo -e "${BOLD_WHITE}USAGE:${NC}"
    echo -e "  ./run_tests.sh                    Run all tests with Sisyphus tracking"
    echo -e "  ./run_tests.sh story              View your Sisyphus story progress"
    echo -e "  ./run_tests.sh help               Show this help message"
    echo ""
    echo -e "${BOLD_WHITE}FEATURES:${NC}"
    echo -e "  ${CYAN}📊 Progress Tracking${NC}      - Automatic tracking of test results over time"
    echo -e "  ${CYAN}🎮 Gamification${NC}          - XP system, levels, and achievements"
    echo -e "  ${CYAN}📜 Story Mode${NC}            - Unlock narrative fragments as you level up"
    echo -e "  ${CYAN}🏛️ Greek Mythology Theme${NC} - Immersive mythological experience"
    echo ""
    echo -e "${BOLD_WHITE}STORY SYSTEM:${NC}"
    echo -e "  - Each level from 1-50 unlocks a story fragment"
    echo -e "  - The story follows Sisyphus and Daedalus building the Labyrinth"
    echo -e "  - 5 acts with 10 levels each, building to a philosophical climax"
    echo -e "  - Story progress is saved in sisyphus.legend"
    echo ""
    echo -e "${BOLD_WHITE}FILES:${NC}"
    echo -e "  ${GRAY}.sisyphus/            Progress archive directory"
    echo -e "  sisyphus.legend       Lifetime stats and story progress"
    echo -e "  .sisyphus/archive/    Historical test run data${NC}"
    echo ""
    exit 0
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
        "👤 THE QUEEN'S SHADOW|You spot a lone figure in royal robes on a distant balcony, her gaze fixed not on you, but on the ground beneath the workshop. 'Queen Pasiphaë,' Daedalus says without looking up. 'She is listening for her son. Pray she never hears him answer."
        "🔒 THE SEALED CHAMBER|Daedalus shows you the blueprint's center—a room marked with symbols you don't recognize. 'This chamber,' he says, his voice tight, 'must never be accessible from the outside. Only from within. The King's specific orders.'"
        "⚔️ THE CAPTAIN'S INSPECTION|A scarred guard captain arrives to review your progress. His eyes linger on your hands as you code. 'I've executed architects before,' he says casually. 'The King values... precision.' Daedalus's hand trembles as he rolls up the blueprints."
        "📐 THE IMPOSSIBLE SPECIFICATION|Daedalus hands you a new requirement scroll. The mathematics make no sense—corridors that loop back on themselves, doors that open both ways simultaneously. 'Make it work,' he says desperately. 'The King requested the logic himself.'"
        "🌙 THE NIGHT SHIFT|As darkness falls, Daedalus lights a single candle and continues working. 'The real work begins now,' he murmurs. 'When the palace sleeps, we build the sections the King must never see.' He slides you a second, hidden blueprint."
    )

    # --- ACT II: THE LABYRINTH'S NATURE (LEVELS 11-20) ---
    # Theme: Rising action. The mystery deepens. The Labyrinth is more than a building,
    # and the characters' true natures begin to surface.
    declare -a act_two_greetings=(
        "🏰 ECHOES OF CORINTH|An archway you design is perfectly proportioned, an echo of a time you built palaces, not prisons. Daedalus pauses. 'That arch,' he says quietly, his voice tinged with something like pity, 'is too beautiful for this place. Minos will not approve. Tear it down and build it again, but this time, build it like a cage.'"
        "🍷 THE ARCHITECT'S TREMOR|Daedalus's hands shake as he draws. Not from wine this time—from something else. He keeps glancing at a locked drawer in his desk. 'Some prisons,' he mutters, 'hold more than monsters. Some hold the reasons we deserve to be here.'"
        "💔 THE WEIGHT OF BETRAYAL|'I know what you did,' Daedalus says suddenly over his cups. 'Cheated death, betrayed the gods' hospitality. But at least your crime was against immortals. Mine...' He stops, staring at his hands. 'Mine was against someone who trusted me.'"
        "🪶 A CHILD'S DRAWING|While organizing blueprints, a small parchment falls out—a child's drawing of a bird-man flying over the sea. Daedalus snatches it away, but not before you see the inscription: 'For Papa, when we fly together - I.' His eyes are wild with panic."
        "👑 SISYPHUS'S CONFESSION|'I used to watch my workers build,' you tell Daedalus, surprising yourself. 'Every stone placed was a choice, a small rebellion against entropy. Now I build prisons. The gods' joke—turning creation into damnation.'"
        "🔨 THE NEPHEW'S SHADOW|Daedalus flinches when you use a particularly clever technique. 'Perdix used to do that,' he whispers. 'My nephew. He was brilliant. Too brilliant.' His hand unconsciously moves to his chest, as if feeling for an old wound."
        "🐂 THE BEAST GROWS RESTLESS|The roars from below have changed—no longer just rage, but something worse: rhythm. Pattern. 'It's counting,' Daedalus realizes with horror. 'Three roars, pause. Seven roars, pause. It's learning mathematics. Your code is teaching it to think.'"
        "⚖️ THE BUILDER'S BURDEN|'We're alike, you and I,' Daedalus says, watching you debug with inhuman persistence. 'Both punished for our cleverness. Both building our own hells. The difference is...' He pauses, touching that locked drawer again. 'I still have something left to lose.'"
        "🗿 THE LIVING STONE|You place your hand on a completed wall and feel a pulse—not mechanical, but organic. 'It's learning from us,' you realize aloud. 'Our guilt, our regret, our...' Daedalus finishes: 'Our desperate need to protect what we love. Yes. The Labyrinth knows.'"
        "🌅 A PROMISE IN CODE|Late at night, Daedalus shows you a function he's written—beautiful, elegant, purposeless. 'For after,' he says cryptically. 'When the spring winds come. When young birds test their wings.' He deletes it immediately, but you memorize every line."
    )

    # --- ACT III: THE CRACKS APPEAR (LEVELS 21-30) ---
    # Theme: The midpoint. The characters begin to break under the strain.
    # The true, monstrous scale of the project becomes clear.
    declare -a act_three_greetings=(
        "🎭 THE MASK SLIPS|Daedalus hasn't slept in three days. He's muttering to blueprints, arguing with walls. 'They're wrong!' he screams suddenly. 'The calculations! Someone changed them!' You check—he's right. The Labyrinth is rewriting itself."
        "⚡ THE FIRST REBELLION|You deliberately introduce a bug—a way out, disguised as a memory leak. Your cursor hovers over the delete key. For the first time in eternity, you choose defiance. The comment reads: '// Some errors are features.'"
        "🔥 THE BEAST BREAKS FREE|Screams echo through the palace. The Minotaur has breached the inner walls. Guards flee as stone and bronze buckle under inhuman strength. 'It's too strong!' Daedalus cries. 'The rage is evolving faster than our prison!'"
        "🌙 A MOTHER'S LULLABY|The rampage stops abruptly. Through the chaos, you hear it—a woman's voice, singing softly. The beast's roars fade to whimpers. Guards whisper of the Queen walking alone into the ruins, emerging unharmed. 'Even monsters,' Daedalus says quietly, 'remember their mothers.'"
        "🪶 THE WORKSHOP BETRAYAL|You find a half-built wing frame hidden in Daedalus's quarters. But it's child-sized. 'You're building escape for someone,' you confront him. His eyes fill with tears. 'The King has my son. Everything I build... it's all for Icarus.'"
        "🗡️ THE CAPTAIN'S SUSPICION|The guard captain corners you alone. 'Architects who whisper at night often whisper their last,' he warns. 'Whatever conspiracy you're weaving into those walls... I'm watching.' His hand rests on his sword."
        "💀 THE HORROR OF HAPPINESS|You fix a particularly complex bug and a wave of pure, intellectual satisfaction washes over you. And then the horror hits: you were happy. You found joy in polishing a gear in a machine of death. The realization is a heavier blow than any boulder. You stare at your hands, not as a builder, but as a monster's accomplice."
        "📜 THE KING'S NEW DEMAND|Minos arrives unannounced, Theseus's name on his lips like poison. 'Change everything,' he commands. 'Make it crueler. If heroes are coming, let them find not a maze, but a meat grinder.' Daedalus's stylus snaps in his grip."
        "🌊 THE POINT OF NO RETURN|'We could stop,' Daedalus whispers one night. 'Refuse. Let them kill us.' You both know it's a lie. The work has consumed you. The Labyrinth isn't just your prison—it's your child. And like all parents, you're damned to see it complete."
        "🔮 THE ORACLE'S WARNING|A priestess arrives with a prophecy: 'Two shall enter the workshop as builders. Only one shall leave as a man. The other...' She looks at you with pity. 'The other shall leave as something else entirely.'"
    )

    # --- ACT IV: INESCAPABLE TRUTHS (LEVELS 31-40) ---
    # Theme: The climax. The major secrets are revealed, confronting the characters with the full horror of their situation.
    declare -a act_four_greetings=(
        "🤝 THE DESPERATE PROPOSITION|Daedalus grabs your arm, his grip iron-strong with desperation. 'I know about your hidden exits. You know about my son. Help me get Icarus out, and I'll ensure your code saves every tribute who enters. We can beat them all—Minos, the gods, fate itself.'"
        "🤔 SISYPHUS'S DOUBT|You study Daedalus in the candlelight. This man murdered his nephew. His 'accidents' destroy lives. Can you trust someone whose guilt runs deeper than yours? But then you remember: even damned souls can choose redemption."
        "💭 THE WEIGHT OF DECISION|Three days you deliberate. Daedalus grows frantic, leaving blueprints of wings on your desk. 'We fly over the labyrinth,' he whispers. 'Land on the tower where they keep him. The guards won't expect an assault from above.' It's madness. It might work."
        "✊ THE PACT IS SEALED|'I'll help you,' you finally say. Daedalus weeps—the first honest tears you've seen. 'But we do this my way. Every function we write saves a life. Every bug we leave helps a hero. If we're going to break the rules, we break them all.'"
        "🪶 STEALING DIVINITY|You begin collecting materials. Feathers from the sacred birds. Wax from the temple candles. Bronze gears from the Labyrinth's mechanisms. 'We're not just building wings,' Daedalus explains. 'We're stealing the gods' own tools of flight.'"
        "🔧 THE SECRET WORKSHOP|Behind a false wall, you've built a second laboratory. Here, physics bends to desperate will. The wings take shape—part engineering, part prayer. 'Each feather must be perfect,' Daedalus insists. 'One flaw and Icarus falls.'"
        "📐 SISYPHUS'S MATHEMATICS|Your curse becomes crucial. You calculate wind patterns obsessively, test weight distributions endlessly. 'I can push this boulder forever,' you realize. 'I can perfect these wings through infinite iteration.' Daedalus watches in awe as you refine the impossible."
        "🌙 THE GUARD ROTATIONS|You've mapped every sentry's pattern. The tribute ships arrive in seven days—chaos will reign. 'During the ceremony,' Daedalus plans, 'all eyes on the maze entrance. We launch from the north tower. Fifteen minutes to reach Icarus.'"
        "🎭 THE TWO LABYRINTHS|By day, you build the Labyrinth Minos desires. By night, you build a second, invisible one within it—a maze of hope only a true hero could navigate. Daedalus looks over your shoulder, his face pale. 'This is a dangerous game,' he whispers. You don't look up from the code. 'I have an eternity to play it.'"
        "⚡ TOMORROW WE FLY|The wings are complete. Tested in secret midnight flights from the tower. Daedalus trembles with hope and terror. 'Tomorrow, when the tributes arrive, we strike. Icarus will be free.' He pauses. 'We fly at dawn, before the sun rises too high.'"
    )

    # --- ACT V: THE ETERNAL CYCLE (LEVELS 41-50) ---
    # Theme: The resolution. The acceptance of the absurd. The Labyrinth is "done," but the work never ends.
    declare -a act_five_greetings=(
        "🌅 THE DAWN OF FREEDOM|The tribute ships arrive in chaos. Guards rush to the harbor. You and Daedalus climb the north tower, Icarus between you, wings strapped tight. 'Remember,' Daedalus tells his son, 'not too high.' The boy nods, eyes bright with dreams of sky."
        "🕊️ THE IMPOSSIBLE FLIGHT|They leap. By all laws of gods and men, they should plummet. But they rise. Icarus laughs—a sound of pure, defiant joy. For one perfect moment, you've beaten them all. The boy soars higher, drunk on freedom, reaching for the sun..."
        "💔 THE FALL AND THE SILENCE|Icarus falls. Daedalus screams. The sound tears the morning apart—a father's soul breaking. Then... nothing. No splash. No crash. Just absence. When you look again, the sky is empty. Both of them—gone. You stand alone on the tower."
        "🏛️ THE KING'S GAMBIT|You return to find Minos at your terminal, a single feather resting on the keyboard. 'So, the architect escapes,' he says, his voice devoid of anger. 'A calculated loss. But you, the immortal engine... you remain.' He smiles, a terrible, knowing smile. 'The Labyrinth still requires a caretaker. And now, Sisyphus, you have my undivided attention.'"
        "⚖️ THE DEVIL'S BARGAIN|'Here's my offer,' Minos says. 'Maintain the Labyrinth. Make it kinder. Save who you can. In return, I won't hunt Daedalus.' He smiles coldly. 'Refuse, and I'll make the maze a slaughterhouse. Your choice, eternal one.'"
        "✨ THE REVELATION|As your fingers find the keys, you understand: you were never meant to escape. Daedalus built wings. You built purpose. The gods didn't curse you—they prepared you. For this. For choosing to stay when you could have fled."
        "📝 THE CODER'S MANIFESTO|You write a new function: compassion.exe. Then another: hope.dll. The Labyrinth becomes your canvas. Each algorithm is a small kindness, each bug fix a life saved. You're not pushing a boulder. You're sculpting salvation."
        "🌊 THE MYTH REWRITES ITSELF|Years pass. Tales spread of the Labyrinth that helps heroes, the maze that teaches rather than kills. They whisper of a ghost in the code—Sisyphus, who chose his chains and transformed them into wings for others."
        "💪 THE HAPPY SISYPHUS|You push back from your terminal and laugh—a real, deep laugh. The gods wanted to break you with eternal labor. Instead, you've become unbreakable. The boulder rolls down. You crack your knuckles. 'Again,' you say, and mean it."
        "♾️ ONE MUST IMAGINE SISYPHUS CODING|The sun sets. It rises. The bugs appear. You fix them. But now you know the secret—in the space between the bug and the fix, in that moment of problem-solving, you are completely, perfectly, eternally free. The cursor blinks. You smile. Time to push the boulder up the hill again. And you wouldn't have it any other way."
    )
