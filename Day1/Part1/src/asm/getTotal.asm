    assume adl=1

    section .text

include '../../include/ti84pceg.inc'

    public _getTotal

offsetPointer := ti.appData
offset := offsetPointer + 3
size := offset + 3

_getTotal:
    push ix
    ld ix, 0
    add ix, sp
    ld hl, (ix + 6) ; offset in appvar to begin searching
    pop ix
    ld (offsetPointer), hl
    ld hl, (hl)
    ld (offset), hl
    ld hl, inputVar
    call ti.Mov9ToOP1
    call ti.ChkFindSym
    ret c
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
    ld hl, 0
    ld a, (de)
    ld l, a
    inc de
    ld a, (de)
    ld h, a
    ld (size), hl
    inc de ; we have arrived at the data pointer!
    ld hl, (offset)
    add hl, de
    ex de, hl
    ld bc, 0
    push bc ; current total
    push bc ; current number

.loop:
    ld a, (de)
    or a, a ; check for end of list
    jr z, .return
    sub a, $30
    ld hl, 0
    ld l, a
    pop bc
    add hl, bc
    push hl ; update current number
    inc de ; inc data pointer
    ld hl, (offset)
    inc hl
    ld (offset), hl ; inc offset
    ld bc, (size)
    or a, a
    sbc hl, bc
    jr z, .return
    ld a, (de)
    or a, a
    jr z, .nextNumber
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
    pop bc ; current number
    pop hl ; total number
    add hl, bc
    push hl ; update total number
    ld bc, 0
    push bc ; reset current number
    inc de ; next number 
    ld hl, (offset)
    inc hl
    ld (offset), hl
    ld bc, (size)
    or a, a
    sbc hl, bc
    jr z, .return
    jr .loop

.return:
    ld de, (offset)
    inc de ; skip the 0 we ended on (I totally didn't almost forget this)
    ld hl, (offsetPointer)
    ld (hl), de
    pop hl ; remove current number
    pop hl ; total
    ret

inputVar:
    db ti.AppVarObj, 'Input', 0
