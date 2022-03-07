# CLNotes

## Description
CLNotes is a (crud) command line tool that helps you keep track of your notes/todos using the command line, like a pro(grammer) B-)

## Installation
```
cd
git clone https://github.com/TomiG06/CLNotes
cd CLNotes
./setup.sh
```

## Commands
* `-c [new instance (string)]`
* `-r [status, . or blank(all)]`
* `-u [number/s to check/uncheck]`
* `-d [number/s to delete, .(all), status]`
* `-h`

## Notes

* Inside the program, words 'note', 'instance' and 'todo' refer to the same thing
* Exit status 1 does not take place when user enters illegal number arguments on `-d` and `-u` (illegal meaning numbers out of range and strings containing non digit characters). Might modify in the future to ignore them or give exit status 1 when meeted.