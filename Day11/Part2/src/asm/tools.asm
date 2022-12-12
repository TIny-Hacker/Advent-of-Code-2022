    assume adl=1

    section .text

include '../../include/ti84pceg.inc'

    public _monkeyCount
    public _nextMonkey
    public _getNumber

offsetPointer := ti.appData
offset := offsetPointer + 3

EOF := offset + 3

negativeFlag := 0

_monkeyCount:
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
    ld hl, 0
    ld a, (de)
    ld l, a
    inc de
    ld a, (de)
    ld h, a ; get program size
    inc de
    call _getEOF
    push de
    pop bc
    ld de, 0
    push de

.loop:
    call _checkEOF
    jr z, .return
    ld a, (bc)
    cp a, 'M'
    inc bc
    jr nz, .loop
    pop de
    inc de
    push de
    jr .loop

.return:
    pop hl
    ret

_nextMonkey:
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
    push hl
    inc hl
    ex de, hl

.loop:
    ld a, (de)
    cp a, 'M'
    jr z, .return
    inc de
    ld bc, (offset)
    inc bc
    ld (offset), bc
    ld hl, (offsetPointer)
    ld (hl), bc
    jr .loop

.return:
    ld bc, (offset)
    inc bc
    ld (offset), bc
    ld hl, (offsetPointer)
    ld (hl), bc
    pop hl
    ret

_getNumber:
    ld iy, ti.flags
    push ix
    ld ix, 0
    add ix, sp
    ld de, (ix + 6) ; pointer to start of number
    pop ix

.loopFind:
    ld a, (de)
    cp a, '0'
    inc de
    jr c, .loopFind
    cp a, ':'
    jr nc, .loopFind
    dec de
    ld bc, 0
    push bc

.loopNumber:
    ld a, (de)
    sub a, $30
    ld hl, 0
    ld l, a
    pop bc
    add hl, bc
    inc de
    ld a, (de)
    cp a, '0'
    ret c
    cp a, ':'
    ret nc
    push hl
    pop bc
    add hl, hl
    add hl, hl
    add hl, bc
    add hl, hl
    push hl
    jr .loopNumber

_checkEOF: ; bc = current address being read; destroys hl and a
    ld hl, (EOF)
    inc hl
    or a, a
    sbc hl, bc
    ret nc
    dec de
    cp a, a
    ret

_getEOF: ; args: hl = size of var; de = start of variable; preserves both registers
    push hl
    dec hl
    add hl, de
    ld (EOF), hl
    pop hl
    ret

inputVar:
    db ti.AppVarObj, 'Input', 0
