#!/usr/bin/env python3
import sys
from string import Template
from enum import Enum
from os import path
from typing import NamedTuple, Tuple, Optional, Union, Dict, List


this_dir = path.dirname(path.abspath(__file__))


class Matrix(Enum):
    """ 
    Enum defining logical row/col names and their names in the PinsState struct
    """
    row0 = 'rows[0]'
    row1 = 'rows[1]'
    row2 = 'rows[2]'
    row3 = 'rows[3]'
    row4 = 'rows[4]'
    row5 = 'rows[5]'
    row6 = 'rows[6]'
    row7 = 'rows[7]'

    col0 = 'cols[0]'
    col1 = 'cols[1]' 
    col2 = 'cols[2]' 
    col3 = 'cols[3]' 
    col4 = 'cols[4]' 
    col5 = 'cols[5]' 
    col6 = 'cols[6]' 
    col7 = 'cols[7]' 
    k0 = 'cols[8]'
    k1 = 'cols[9]'
    k2 = 'cols[10]'

    restore = 'restore'
    forty_eighty = 'forty_eighty'
    caps_lock = 'caps_lock'


class KeyInfo(NamedTuple):
    # USB keyboard key name, used to define constants
    name: str           

    # C128 keyboard key name, used for logging output
    c128_name: str      

    # C128 keybord matrix coordinates (ROW, COL), None if not sent to the C128
    matrix: Optional[Union[Tuple[Matrix, Matrix], Matrix]]  


# Dict mapping the USB raw key code to the KeyInfo
key_codes: Dict[int, KeyInfo] = {
    0x04: KeyInfo('a', 'a', (Matrix.row1, Matrix.col2)),
    0x05: KeyInfo('b', 'b', (Matrix.row4, Matrix.col3)),
    0x06: KeyInfo('c', 'c', (Matrix.row4, Matrix.col2)),
    0x07: KeyInfo('d', 'd', (Matrix.row2, Matrix.col2)),
    0x08: KeyInfo('e', 'e', (Matrix.row6, Matrix.col1)),
    0x09: KeyInfo('f', 'f', (Matrix.row5, Matrix.col5)),
    0x0a: KeyInfo('g', 'g', (Matrix.row5, Matrix.col5)),
    0x0b: KeyInfo('h', 'h', (Matrix.row5, Matrix.col3)),
    0x0c: KeyInfo('i', 'i', (Matrix.row1, Matrix.col4)),
    0x0d: KeyInfo('j', 'j', (Matrix.row2, Matrix.col4)),
    0x0d: KeyInfo('k', 'k', (Matrix.row5, Matrix.col4)),
    0x0f: KeyInfo('l', 'l', (Matrix.row2, Matrix.col5)),
    0x10: KeyInfo('m', 'm', (Matrix.row4, Matrix.col4)),
    0x11: KeyInfo('n', 'n', (Matrix.row7, Matrix.col4)),
    0x12: KeyInfo('o', 'o', (Matrix.row6, Matrix.col4)),
    0x13: KeyInfo('p', 'p', (Matrix.row1, Matrix.col5)),
    0x14: KeyInfo('q', 'q', (Matrix.row6, Matrix.col7)),
    0x15: KeyInfo('r', 'r', (Matrix.row1, Matrix.col2)),
    0x16: KeyInfo('s', 's', (Matrix.row5, Matrix.col1)),
    0x17: KeyInfo('t', 't', (Matrix.row6, Matrix.col2)),
    0x18: KeyInfo('u', 'u', (Matrix.row6, Matrix.col3)),
    0x19: KeyInfo('v', 'v', (Matrix.row7, Matrix.col3)),
    0x1a: KeyInfo('w', 'w', (Matrix.row1, Matrix.col1)),
    0x1b: KeyInfo('x', 'x', (Matrix.row7, Matrix.col2)),
    0x1c: KeyInfo('y', 'y', (Matrix.row1, Matrix.col3)),
    0x1d: KeyInfo('z', 'z', (Matrix.row4, Matrix.col1)),

    0x1e: KeyInfo('1', '1', (Matrix.row0, Matrix.col7)),
    0x1f: KeyInfo('2', '2', (Matrix.row3, Matrix.col7)),
    0x20: KeyInfo('3', '3', (Matrix.row0, Matrix.col1)),
    0x21: KeyInfo('4', '4', (Matrix.row3, Matrix.col1)),
    0x22: KeyInfo('5', '5', (Matrix.row0, Matrix.col2)),
    0x23: KeyInfo('6', '6', (Matrix.row3, Matrix.col2)),
    0x24: KeyInfo('7', '7', (Matrix.row0, Matrix.col3)),
    0x25: KeyInfo('8', '8', (Matrix.row3, Matrix.col3)),
    0x26: KeyInfo('9', '9', (Matrix.row0, Matrix.col4)),
    0x27: KeyInfo('0', '0', (Matrix.row3, Matrix.col4)),

    0x28: KeyInfo('ENTER',      'RETURN',   (Matrix.row1, Matrix.col0)),
    0x29: KeyInfo('ESC',        'RUN/STOP', (Matrix.row7, Matrix.col7)),
    0x2a: KeyInfo('BACKSPACE',  'INS/DEL',  (Matrix.row0, Matrix.col0)),
    0x2b: KeyInfo('TAB',        'CTRL',     (Matrix.row2, Matrix.col7)),
    0x2c: KeyInfo('SPACE',      'SPACE',    (Matrix.row4, Matrix.col7)),
    0x2d: KeyInfo('MINUS',      '+',        (Matrix.row0, Matrix.col5)),
    0x2e: KeyInfo('EQUAL',      '-',        (Matrix.row3, Matrix.col5)),
    0x2f: KeyInfo('LBRACKET',   '@',        (Matrix.row6, Matrix.col5)),
    0x30: KeyInfo('RBRACKET',   '*',        (Matrix.row1, Matrix.col6)),
    0x31: KeyInfo('BACKSLASH',  '=',        (Matrix.row5, Matrix.col6)),
    0x33: KeyInfo('SEMICOLON',  '[',        (Matrix.row5, Matrix.col5)),
    0x34: KeyInfo('APOSTROPHE', ']',        (Matrix.row2, Matrix.col6)),
    0x35: KeyInfo('GRAVE',      '←',        (Matrix.row1, Matrix.col7)),
    0x36: KeyInfo('COMMA',      ',',        (Matrix.row7, Matrix.col5)),
    0x37: KeyInfo('DOT',        '.',        (Matrix.row4, Matrix.col5)),
    0x38: KeyInfo('SLASH',      '/',        (Matrix.row7, Matrix.col6)),
    0x39: KeyInfo('CAPS_LOCK', None, None),

    0x3a: KeyInfo('F1',  'ESC',       (Matrix.row0, Matrix.k1)),
    0x3b: KeyInfo('F2',  'TAB',       (Matrix.row3, Matrix.k0)),
    0x3c: KeyInfo('F3',  'ALT',       (Matrix.row0, Matrix.k2)),
    0x3d: KeyInfo('F4',  'CAPS_LOCK', Matrix.caps_lock),
    0x3e: KeyInfo('F5',  'HELP',      (Matrix.row0, Matrix.k0)),
    0x3f: KeyInfo('F6',  'LINE_FEED', (Matrix.row3, Matrix.k1)),
    0x40: KeyInfo('F7',  '40/80',     Matrix.forty_eighty),
    0x41: KeyInfo('F8',  'NO_SCROLL', (Matrix.row7, Matrix.k2)),
    0x42: KeyInfo('F9',  'F1',        (Matrix.row4, Matrix.col0)),
    0x43: KeyInfo('F10', 'F3',        (Matrix.row5, Matrix.col0)),
    0x44: KeyInfo('F11', 'F5',        (Matrix.row6, Matrix.col0)),
    0x45: KeyInfo('F12', 'F7',        (Matrix.row3, Matrix.col0)),

    0x49: KeyInfo('INSERT', '£',        (Matrix.row0, Matrix.col6)),
    0x4a: KeyInfo('HOME',   'CLR/HOME', (Matrix.row3, Matrix.col6)),
    0x4b: KeyInfo('PGUP',   'RESTORE',  Matrix.restore),
    0x4c: KeyInfo('DELETE', '↑',        (Matrix.row6, Matrix.col6)),

    0x4f: KeyInfo('RIGHT', 'CURS_RL',  (Matrix.row2, Matrix.col0)),
    0x50: KeyInfo('LEFT',  'CURS_RL',  (Matrix.row2, Matrix.col0)),
    0x51: KeyInfo('DOWN',  'CURS_DU',  (Matrix.row7, Matrix.col0)),
    0x52: KeyInfo('UP',    'CURS_DU',  (Matrix.row7, Matrix.col0)),

    0x53: KeyInfo('NUMLOCK',   None, None),
    0x56: KeyInfo('KP_MINUS', 'KP-',      (Matrix.row2, Matrix.k1)),
    0x57: KeyInfo('KP_PLUS',  'KP+',      (Matrix.row1, Matrix.k1)),
    0x58: KeyInfo('KP_ENTER', 'KP_ENTER', (Matrix.row4, Matrix.k1)),
    0x59: KeyInfo('KP_0',     'KP_0',     (Matrix.row1, Matrix.k2)),
    0x5a: KeyInfo('KP_1',     'KP_1',     (Matrix.row7, Matrix.k0)),
    0x5b: KeyInfo('KP_2',     'KP_2',     (Matrix.row4, Matrix.k0)),
    0x5c: KeyInfo('KP_3',     'KP_3',     (Matrix.row7, Matrix.k1)),
    0x5d: KeyInfo('KP_4',     'KP_4',     (Matrix.row5, Matrix.k0)),
    0x5e: KeyInfo('KP_5',     'KP_5',     (Matrix.row2, Matrix.k0)),
    0x5f: KeyInfo('KP_6',     'KP_6',     (Matrix.row5, Matrix.k1)),
    0x60: KeyInfo('KP_7',     'KP_7',     (Matrix.row6, Matrix.k0)),
    0x61: KeyInfo('KP_8',     'KP_8',     (Matrix.row1, Matrix.k0)),
    0x62: KeyInfo('KP_9',     'KP_9',     (Matrix.row6, Matrix.k1)),
    0x63: KeyInfo('KP_DOT',   'KP_.',     (Matrix.row2, Matrix.k2)),

    0x65: KeyInfo('COMPOSE', None, None),
    0x67: KeyInfo('LCTRL',  'CBM',    (Matrix.row5, Matrix.col7)),
    0x68: KeyInfo('LSHIFT', 'LSHIFT', (Matrix.row7, Matrix.col1)),
    0x69: KeyInfo('LALT', 'ALT',      (Matrix.row0, Matrix.k2)),
    0x6a: KeyInfo('LMETA', None, None),
    0x6c: KeyInfo('RSHIFT', 'RSHIFT', (Matrix.row4, Matrix.col6)),
    0x6b: KeyInfo('RCTRL', None, None),
    0x6d: KeyInfo('RALT', 'ALT',      (Matrix.row0, Matrix.k2)),
    0x6e: KeyInfo('RMETA', None, None),
}

numlock_off_keycodes: Dict[int, KeyInfo] = {
    0x5b: KeyInfo('KP_2', 'TOP_ROW_↓', (Matrix.row4, Matrix.k2)),
    0x5d: KeyInfo('KP_4', 'TOP_ROW_←', (Matrix.row5, Matrix.k2)),
    0x5f: KeyInfo('KP_6', 'TOP_ROW_→', (Matrix.row6, Matrix.k2)),
    0x61: KeyInfo('KP_8', 'TOP_ROW_↑', (Matrix.row3, Matrix.k2)),
}

def output_pin_name(pin: Matrix) -> str:
    return pin.name.upper() + '_PIN'


def usb_key_name(key_name: str) -> str:
    return f'USB_KEY_{key_name.upper()}'


def main(args: List[str]) -> int:
    header_comment = f'This file was generated by scripts/{path.basename(__file__)}'
    max_usb_key_code = max(key_codes.keys())

    """ ---- Generate key_depfs.hpp """
    with open(path.join(this_dir, 'key_defs.hpp.template')) as f:
        key_mapping_cpp_template = Template(f.read())
    
    usb_key_defs: str = ''
    for key_code, key_info in key_codes.items():
        usb_key_defs += '#define {0:18} 0x{1:02x}\n'.format(usb_key_name(key_info.name), key_code)

    with open(path.join(this_dir, '..', 'lib', 'KeyMapping', 'key_defs.hpp'), 'w') as f:
        f.write(key_mapping_cpp_template.substitute(
            header_comment=header_comment,
            max_usb_key_code=hex(max_usb_key_code),
            usb_key_defs=usb_key_defs
        ))

    """ ---- Generate key_mapping.cpp """
    with open(path.join(this_dir, 'key_mapping.cpp.template')) as f:
        key_mapping_template = Template(f.read())

    usb_key_mapping = ''
    for key_code_num in range(0, max_usb_key_code + 1):
        usb_key_mapping += '    /* 0x{0:02x} */  '.format(key_code_num)
        if key_code_num in key_codes and key_codes[key_code_num].matrix:
            key_info = key_codes[key_code_num]

            if type(key_info.matrix) == tuple:
                output_pins_ref = f'{key_info.matrix[0].value}.{key_info.matrix[1].value}'
            else:
                output_pins_ref = f'special.{key_info.matrix.value}'

            mapping = f'{{true, &output_pins_state.{output_pins_ref}}},'
            usb_key_mapping += '{0:32} // {1} => {2}\n'.format(mapping, usb_key_name(key_info.name), key_info.c128_name)
        else:
            usb_key_mapping += '{false, nullptr},\n'
    
    numlock_off_key_mapping = ''
    for key_code_num, key_info in numlock_off_keycodes.items():
        numlock_off_key_mapping += f'KeyInfo numlock_off_{key_info.name.lower()} {{true, &output_pins_state.{key_info.matrix[0].value}.{key_info.matrix[1].value}}};'
        numlock_off_key_mapping += f'     // {usb_key_name(key_info.name)} => {key_info.c128_name}\n'


    with open(path.join(this_dir, '..', 'lib', 'KeyMapping', 'key_mapping.cpp'), 'w') as f:
        f.write(key_mapping_template.substitute(
            header_comment=header_comment,
            usb_key_mapping=usb_key_mapping,
            numlock_off_key_mapping=numlock_off_key_mapping
        ))

    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
