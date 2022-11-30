# Advent-of-Code-2022
My solutions for Advent of Code 2022, on the TI-84 Plus CE Graphing Calculator. I'm not really that great of a programmer, so I probably won't do everything.

### grabInput.sh

This is a little script I made to download a specified day's input and convert it to an 8xv file to send to the calculator. It also replaces all newlines (`0x0A`) with nulls (`0x00`) to make life easier. Syntax is as follows:

    grabInput.sh <year> <day>

It will create a directory called "Day\<day\>", with a converted `input.txt` and `Input.8xv`. It requires a file `cookies.txt` to be in the same directory as the script, which has the necessary info to allow you to properly download the input. You can generate a `cookies.txt` file with the cookies.txt extension for [Chrome](https://chrome.google.com/webstore/detail/get-cookiestxt/bgaddhkoddajcdgocldbbfleckgcbcid) or [Firefox](https://addons.mozilla.org/en-US/firefox/addon/cookies-txt/). The script uses `wget`, `tr`, and `convbin`. There is little to no error checking, so do whatever you want but don't get mad if something goes wrong 😆
