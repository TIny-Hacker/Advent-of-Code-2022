    assume adl=1

    section .text

include '../../include/ti84pceg.inc'

    public _getNumbers

offsetPointer := ti.appData
offset := offsetPointer + 3

_getNumbers:
    push ix
    pop de
    ld ix, 0
    add ix, sp
    ld hl, (ix + 3) ; offset in appvar to begin searching
    ld bc, (ix + 15) ; high2
    push bc
    ld bc, (ix + 12) ; low2
    push bc
    ld bc, (ix + 9) ; high1
    push bc
    ld bc, (ix + 6) ; low1
    push bc
    push de
    pop ix
    ld (offsetPointer), hl
    ld hl, (hl)
    ld (offset), hl
    ld hl, inputVar
    call ti.Mov9ToOP1
    call ti.ChkFindSym
    ret c ; no input file found
    call ti.ChkInRam
    jr z, .inRam
    ld hl, 10
    add hl, de
    ld a, c
    ld bc, 0
    ld c, a
    add hl, bc
    ex de, hl

.inRam:
    inc de
    inc de ; skip size bytes
    ld hl, (offset)
    add hl, de ; skip offset
    push hl ; pointer to first number
    call ti.StrLength
    ld hl, (offset)
    add hl, bc ; update offset
    inc hl
    ld de, (offsetPointer)
    ex de, hl
    ld (hl), de
    pop de ; first number
    ld bc, 0
    push bc

.loop:
    ld a, (de)
    sub a, $30 ; normal number
    ld hl, 0
    ld l, a
    pop bc
    add hl, bc
    push hl ; update current number
    inc de ; inc data pointer
    ld a, (de)
    cp a, $30 ; check if it's a number or not
    jr c, .nextNumber
    pop hl
    push hl
    pop bc ; put the current number in both hl and bc
    add hl, hl
    add hl, hl
    add hl, bc
    add hl, hl ; multiply current number by ten
    push hl ; move curent number to the top of the stack
    jr .loop

.nextNumber:
    pop bc
    pop hl
    ld (hl), bc
    ld bc, 0
    push bc
    ld a, (de)
    or a, a
    jr z, .return
    inc de ; next number
    jr .loop

.return:
    pop bc
    ret

inputVar:
    db ti.AppVarObj, 'Input', 0
