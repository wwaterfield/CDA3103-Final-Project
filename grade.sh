DIFF_CMD="colordiff -w"
TIMEOUT="gtimeout 2"

black()   { echo "$(tput setaf 0)$*$(tput sgr0)"; }
red()     { echo "$(tput setaf 1)$*$(tput sgr0)"; }
green()   { echo "$(tput setaf 2)$*$(tput sgr0)"; }
yellow()  { echo "$(tput setaf 3)$*$(tput sgr0)"; }
blue()    { echo "$(tput setaf 4)$*$(tput sgr0)"; }
magenta() { echo "$(tput setaf 5)$*$(tput sgr0)"; }
cyan()    { echo "$(tput setaf 6)$*$(tput sgr0)"; }
white()   { echo "$(tput setaf 7)$*$(tput sgr0)"; }

command -v colordiff >/dev/null 2>&1 || { echo -e "$(red ✕) colordiff is not installed: diff output will be super hard to read\n"; DIFF_CMD="diff"; }
command -v gtimeout >/dev/null 2>&1 || { TIMEOUT="timeout 2"; }
command -v $TIMEOUT >/dev/null 2>&1 || { echo -e "$(red ✕) timeout is not installed: script will hang if a project has an infinite loop\n"; TIMEOUT=""; }

PERFECT_COUNT=0
FLAWED_COUNT=0

GLOB=$1

if [[ "$GLOB" == "--skip-perfect" ]]; then
	GLOB=""
fi

for f in submissions/$GLOB*.c; do
	ln -fs $f project.c;
	COMPILE_SUCCESS=false
	EXECUTE_SUCCESS=false
	OUTPUT_SUCCESS=false

	DIFF=""
	TEST=""
	COMPILATION=$((gcc -w -fdiagnostics-color=always project.c spimcore.c -o project.out) 2>&1)

	if [ $? -eq 0 ]; then
		COMPILE_SUCCESS=true

		OUTPUT=$(echo -e "c\nr\nm\nx" | $TIMEOUT ./project.out input_file.asc | grep '0')

		if [ $? -eq 0 ]; then
			EXECUTE_SUCCESS=true

			DIFF=$($DIFF_CMD <(echo "$OUTPUT") correct_output.txt)
			if [[ $DIFF == "" ]]; then
				OUTPUT_SUCCESS=true
			else
				gcc -w tests.c -o tests.out
				TEST=$(./tests.out)
			fi
		fi
	fi

	if $COMPILE_SUCCESS && $OUTPUT_SUCCESS && $EXECUTE_SUCCESS && [[ $1 == "--skip-perfect" ]]; then
		continue
	fi

	if $COMPILE_SUCCESS && $OUTPUT_SUCCESS && $EXECUTE_SUCCESS; then
		green `basename $f`
		PERFECT_COUNT=$(($PERFECT_COUNT+1))
	elif ! $COMPILE_SUCCESS; then
		red `basename $f`
		FLAWED_COUNT=$(($FLAWED_COUNT+1))
	else
		yellow `basename $f`
		FLAWED_COUNT=$(($FLAWED_COUNT+1))
	fi

	if $COMPILE_SUCCESS; then
		echo "$(green ✓) Compiled"

		if $EXECUTE_SUCCESS; then
			if $OUTPUT_SUCCESS; then
				echo "$(green ✓) Perfect output"
			else
				echo "$(red ✕) Output failure"
				echo "$DIFF"
				echo "$TEST"
			fi
		else
			echo "$(red ✕) Execution error or timeout"
		fi
	else
		echo "$(red ✕) Compilation error"
		echo -e "$COMPILATION"
	fi

	echo -ne "\n"
done

echo Done. $PERFECT_COUNT/$(($PERFECT_COUNT+$FLAWED_COUNT)) outputs were perfect.
