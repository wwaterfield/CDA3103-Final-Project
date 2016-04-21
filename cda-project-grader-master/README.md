# CDA 3103 Project Tester

## Usage

### If you're a grader

1. In WebCourses, download all submissions of the project as a ZIP.
2. Place the unzipped `submissions` folder in the root of the directory. This may involve overwriting the existing folder, which is fine.
3. Run `./grade.sh`.

### If you're a student

1. Download this project either by clicking "Download ZIP" or cloning it.
2. Copy your `project.c` file into the `submissions/` folder of this project. It doesn't have to be named `project.c` for it to work.
3. Inside this project, run `./grade.sh`.

## Example output (without color)

```
some-dude-11233331.c
✓ Compiled
✓ Perfect output

john-smith-41414142412j1230j123.c
✓ Compiled
✕ Output failure
3,4c3,4
<  $t0   00000000      $t1   00000000      $t2   00000000      $t3   00000000
<  $t4   00000000      $t5   00000000      $t6   00000000      $t7   00000000
---
>  $t0   00000001      $t1   00000003      $t2   00000004      $t3   00000004
>  $t4   00000000      $t5   00000001      $t6   00200000      $t7   00000000
9c9
<  $pc   00004000      $stat 00000000      $lo   00000000      $hi   00000000
---
>  $pc   00004024      $stat 00000000      $lo   00000000      $hi   00000000
23c23,24
<  04030-0fffc  00000000
---
>  04030-0fff8  00000000
>  0fffc        00000004

steven-petryk12310292u3409-23048_project.c
✕ Compilation error
project.c:226:3: error: use of undeclared identifier 'END'
        }END switch
         ^
1 error generated.

ben-affleck_2624602_50178756_project.c
✕ Compilation error
project.c:134:35: error: expected ';' after expression
            controls->MemWrite = 0
                                  ^
                                  ;
project.c:411:17: error: expected ';' after return statement
        return n
                ^
                ;
2 errors generated.
```
