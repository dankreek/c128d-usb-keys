#!/usr/bin/env python3
import sys
from string import Template
from enum import Enum
from os import path
from typing import NamedTuple, Tuple, Optional, Dict, List


this_dir = path.dirname(path.abspath(__file__))


class Matrix(Enum):
    """ 
    Enum defining logical row/col names and 
    their digital pin numbers on the Teensy 3.6
    """
    Row0 = 22
    Row1 = 21
    Row2 = 20
    Row3 = 15
    Row4 = 18
    Row5 = 17
    Row6 = 16
    Row7 = 19
    Restore = 14
    FortyEighty = 11
    CapsLock = 12

    Col0 = 23
    Col1 = 6
    Col2 = 5
    Col3 = 4
    Col4 = 3
    Col5 = 2
    Col6 = 1
    Col7 = 7
    K0 = 8
    K1 = 9
    K2 = 10
    Restore0 = 13  # Labled as 0


class KeyInfo(NamedTuple):
    # USB keyboard key name, used to define constants
    name: str           

    # C128 keyboard key name, used for logging output
    c128_name: str      

    # C128 keybord matrix coordinates (ROW, COL), None if not sent to the C128
    matrix: Optional[Tuple[Matrix, Matrix]]  


# Dict mapping the USB raw key code to the KeyInfo
key_codes: Dict[int, KeyInfo] = {
    0x04: KeyInfo('a', 'a', (Matrix.Row1, Matrix.Col2)),
    0x05: KeyInfo('b', 'b', (Matrix.Row4, Matrix.Col3)),
    0x06: KeyInfo('c', 'c', (Matrix.Row4, Matrix.Col2)),
    0x07: KeyInfo('d', 'd', (Matrix.Row2, Matrix.Col2)),
    0x08: KeyInfo('e', 'e', (Matrix.Row6, Matrix.Col1)),
    0x09: KeyInfo('f', 'f', (Matrix.Row5, Matrix.Col5)),
    0x0a: KeyInfo('g', 'g', (Matrix.Row5, Matrix.Col5)),
    0x0b: KeyInfo('h', 'h', (Matrix.Row5, Matrix.Col3)),
    0x0c: KeyInfo('i', 'i', (Matrix.Row1, Matrix.Col4)),
    0x0d: KeyInfo('j', 'j', (Matrix.Row2, Matrix.Col4)),
    0x0d: KeyInfo('k', 'k', (Matrix.Row5, Matrix.Col4)),
    0x0f: KeyInfo('l', 'l', (Matrix.Row2, Matrix.Col5)),
    0x10: KeyInfo('m', 'm', (Matrix.Row4, Matrix.Col4)),
    0x11: KeyInfo('n', 'n', (Matrix.Row7, Matrix.Col4)),
    0x12: KeyInfo('o', 'o', (Matrix.Row6, Matrix.Col4)),
    0x13: KeyInfo('p', 'p', (Matrix.Row1, Matrix.Col5)),
    0x14: KeyInfo('q', 'q', (Matrix.Row6, Matrix.Col7)),
    0x15: KeyInfo('r', 'r', (Matrix.Row1, Matrix.Col2)),
    0x16: KeyInfo('s', 's', (Matrix.Row5, Matrix.Col1)),
    0x17: KeyInfo('t', 't', (Matrix.Row6, Matrix.Col2)),
    0x18: KeyInfo('u', 'u', (Matrix.Row6, Matrix.Col3)),
    0x19: KeyInfo('v', 'v', (Matrix.Row7, Matrix.Col3)),
    0x1a: KeyInfo('w', 'w', (Matrix.Row1, Matrix.Col1)),
    0x1b: KeyInfo('x', 'x', (Matrix.Row7, Matrix.Col2)),
    0x1c: KeyInfo('y', 'y', (Matrix.Row1, Matrix.Col3)),
    0x1d: KeyInfo('z', 'z', (Matrix.Row4, Matrix.Col1)),

    0x1e: KeyInfo('1', '1', (Matrix.Row0, Matrix.Col7)),
    0x1f: KeyInfo('2', '2', (Matrix.Row3, Matrix.Col7)),
    0x20: KeyInfo('3', '3', (Matrix.Row0, Matrix.Col1)),
    0x21: KeyInfo('4', '4', (Matrix.Row3, Matrix.Col1)),
    0x22: KeyInfo('5', '5', (Matrix.Row0, Matrix.Col2)),
    0x23: KeyInfo('6', '6', (Matrix.Row3, Matrix.Col2)),
    0x24: KeyInfo('7', '7', (Matrix.Row0, Matrix.Col3)),
    0x25: KeyInfo('8', '8', (Matrix.Row3, Matrix.Col3)),
    0x26: KeyInfo('9', '9', (Matrix.Row0, Matrix.Col4)),
    0x27: KeyInfo('0', '0', (Matrix.Row3, Matrix.Col4)),

    0x28: KeyInfo('ENTER',      'RETURN',   (Matrix.Row1, Matrix.Col0)),
    0x29: KeyInfo('ESC',        'RUN/STOP', (Matrix.Row7, Matrix.Col7)),
    0x2a: KeyInfo('BACKSPACE',  'INS/DEL',  (Matrix.Row0, Matrix.Col0)),
    0x2b: KeyInfo('TAB',        'CTRL',     (Matrix.Row2, Matrix.Col7)),
    0x2c: KeyInfo('SPACE',      'SPACE',    (Matrix.Row4, Matrix.Col7)),
    0x2d: KeyInfo('MINUS',      '+',        (Matrix.Row0, Matrix.Col5)),
    0x2e: KeyInfo('EQUAL',      '-',        (Matrix.Row3, Matrix.Col5)),
    0x2f: KeyInfo('LBRACKET',   '@',        (Matrix.Row6, Matrix.Col5)),
    0x30: KeyInfo('RBRACKET',   '*',        (Matrix.Row1, Matrix.Col6)),
    0x31: KeyInfo('BACKSLASH',  '=',        (Matrix.Row5, Matrix.Col6)),
    0x33: KeyInfo('SEMICOLON',  '[',        (Matrix.Row5, Matrix.Col5)),
    0x34: KeyInfo('APOSTROPHE', ']',        (Matrix.Row2, Matrix.Col6)),
    0x35: KeyInfo('GRAVE',      '←',        (Matrix.Row1, Matrix.Col7)),
    0x36: KeyInfo('COMMA',      ',',        (Matrix.Row7, Matrix.Col5)),
    0x37: KeyInfo('DOT',        '.',        (Matrix.Row4, Matrix.Col5)),
    0x38: KeyInfo('SLASH',      '/',        (Matrix.Row7, Matrix.Col6)),
    0x39: KeyInfo('CAPS_LOCK', None, None),

    0x3a: KeyInfo('F1',  'ESC',       (Matrix.Row0, Matrix.K1)),
    0x3b: KeyInfo('F2',  'TAB',       (Matrix.Row3, Matrix.K0)),
    0x3c: KeyInfo('F3',  'ALT',       (Matrix.Row0, Matrix.K2)),
    0x3d: KeyInfo('F4',  'CAPS_LOCK', (Matrix.CapsLock, Matrix.Restore0)),
    0x3e: KeyInfo('F5',  'HELP',      (Matrix.Row0, Matrix.K0)),
    0x3f: KeyInfo('F6',  'LINE_FEED', (Matrix.Row3, Matrix.K1)),
    0x40: KeyInfo('F7',  '40/80',     (Matrix.FortyEighty, Matrix.Restore0)),
    0x41: KeyInfo('F8',  'NO_SCROLL', (Matrix.Row7, Matrix.K2)),
    0x42: KeyInfo('F9',  'F1',        (Matrix.Row4, Matrix.Col0)),
    0x43: KeyInfo('F10', 'F3',        (Matrix.Row5, Matrix.Col0)),
    0x44: KeyInfo('F11', 'F5',        (Matrix.Row6, Matrix.Col0)),
    0x45: KeyInfo('F12', 'F7',        (Matrix.Row3, Matrix.Col0)),

    0x49: KeyInfo('INSERT', '£',        (Matrix.Row0, Matrix.Col6)),
    0x4a: KeyInfo('HOME',   'CLR/HOME', (Matrix.Row3, Matrix.Col6)),
    0x4b: KeyInfo('PGUP',   'RESTORE',  (Matrix.Restore, Matrix.Restore0)),
    0x4c: KeyInfo('DELETE', '↑',        (Matrix.Row6, Matrix.Col6)),

    0x4f: KeyInfo('RIGHT', 'CURS_RL',  (Matrix.Row2, Matrix.Col0)),
    0x50: KeyInfo('LEFT',  'CURS_RL',  (Matrix.Row2, Matrix.Col0)),
    0x51: KeyInfo('DOWN',  'CURS_DU',  (Matrix.Row7, Matrix.Col0)),
    0x52: KeyInfo('UP',    'CURS_DU',  (Matrix.Row7, Matrix.Col0)),

    0x53: KeyInfo('NUMLOCK',   None, None),
    0x56: KeyInfo('KP_MINUS', 'KP-',      (Matrix.Row2, Matrix.K1)),
    0x57: KeyInfo('KP_PLUS',  'KP+',      (Matrix.Row1, Matrix.K1)),
    0x58: KeyInfo('KP_ENTER', 'KP_ENTER', (Matrix.Row4, Matrix.K1)),
    0x59: KeyInfo('KP_0',     'KP_0',     (Matrix.Row1, Matrix.K2)),
    0x5a: KeyInfo('KP_1',     'KP_1',     (Matrix.Row7, Matrix.K0)),
    0x5b: KeyInfo('KP_2',     'KP_2',     (Matrix.Row4, Matrix.K0)),
    0x5c: KeyInfo('KP_3',     'KP_3',     (Matrix.Row7, Matrix.K1)),
    0x5d: KeyInfo('KP_4',     'KP_4',     (Matrix.Row5, Matrix.K0)),
    0x5e: KeyInfo('KP_5',     'KP_5',     (Matrix.Row2, Matrix.K0)),
    0x5f: KeyInfo('KP_6',     'KP_6',     (Matrix.Row5, Matrix.K1)),
    0x60: KeyInfo('KP_7',     'KP_7',     (Matrix.Row6, Matrix.K0)),
    0x61: KeyInfo('KP_8',     'KP_8',     (Matrix.Row1, Matrix.K0)),
    0x62: KeyInfo('KP_9',     'KP_9',     (Matrix.Row6, Matrix.K1)),
    0x63: KeyInfo('KP_DOT',   'KP_.',     (Matrix.Row2, Matrix.K2)),

    0x65: KeyInfo('COMPOSE', None, None),
    0x67: KeyInfo('LCTRL',  'CBM',    (Matrix.Row5, Matrix.Col7)),
    0x68: KeyInfo('LSHIFT', 'LSHIFT', (Matrix.Row7, Matrix.Col1)),
    0x69: KeyInfo('LALT', 'ALT',      (Matrix.Row0, Matrix.K2)),
    0x6a: KeyInfo('LMETA', None, None),
    0x6c: KeyInfo('RSHIFT', 'RSHIFT', (Matrix.Row4, Matrix.Col6)),
    0x6b: KeyInfo('RCTRL', None, None),
    0x6d: KeyInfo('RALT', 'ALT',      (Matrix.Row0, Matrix.K2)),
    0x6e: KeyInfo('RMETA', None, None),
}

numlock_off_keycodes: Dict[int, KeyInfo] = {
    0x5b: KeyInfo('KP_2', 'TOP_ROW_↓', (Matrix.Row4, Matrix.K2)),
    0x5d: KeyInfo('KP_4', 'TOP_ROW_←', (Matrix.Row5, Matrix.K2)),
    0x5f: KeyInfo('KP_6', 'TOP_ROW_→', (Matrix.Row6, Matrix.K2)),
    0x61: KeyInfo('KP_8', 'TOP_ROW_↑', (Matrix.Row3, Matrix.K2)),
}

def output_pin_name(pin: Matrix) -> str:
    return pin.name.upper() + '_PIN'


def usb_key_name(key_name: str) -> str:
    return f'USB_KEY_{key_name.upper()}'


def main(args: List[str]) -> int:
    header_comment = f'This file was generated by scripts/{path.basename(__file__)}'
    max_usb_key_code = max(key_codes.keys())

    """ ---- Generate library/KeyMapping/output_pins.hpp """
    with open(path.join(this_dir, 'output_pins.hpp.template')) as f:
        output_pins_template = Template(f.read())

    teensy36_output_pins: str = ''
    for output_pin in Matrix:
        teensy36_output_pins += '#define {0:15} {1}\n'.format(output_pin_name(output_pin), output_pin.value)

    with open(path.join(this_dir, '..', 'lib', 'KeyMapping', 'output_pins.hpp'), 'w') as f:
        f.write(output_pins_template.substitute(
            header_comment=header_comment, 
            teensy36_output_pins=teensy36_output_pins,
        ))

    """ ---- Generate key_mapping.hpp """
    with open(path.join(this_dir, 'key_mapping.hpp.template')) as f:
        key_mapping_cpp_template = Template(f.read())
    
    usb_key_defs: str = ''
    for key_code, key_info in key_codes.items():
        usb_key_defs += '#define {0:18} 0x{1:02x}\n'.format(usb_key_name(key_info.name), key_code)

    with open(path.join(this_dir, '..', 'lib', 'KeyMapping', 'key_mapping.hpp'), 'w') as f:
        f.write(key_mapping_cpp_template.substitute(
            header_comment=header_comment,
            max_usb_key_code=hex(max_usb_key_code),
            usb_key_defs=usb_key_defs,
            output_pins_count=len(Matrix)
        ))

    """ ---- Generate key_mapping.cpp """
    with open(path.join(this_dir, 'key_mapping.cpp.template')) as f:
        key_mapping_template = Template(f.read())

    output_pins = ''
    for matrix_pin in Matrix:
        output_pins += 'PinState {0:11} {{false, {1}}};\n'.format(matrix_pin.name, output_pin_name(matrix_pin))

    usb_key_mapping = ''
    for key_code_num in range(0, max_usb_key_code + 1):
        usb_key_mapping += '    /* 0x{0:02x} */  '.format(key_code_num)
        if key_code_num in key_codes and key_codes[key_code_num].matrix:
            key_info = key_codes[key_code_num]
            mapping = f'{{true, &{key_info.matrix[0].name}, &{key_info.matrix[1].name}}},'

            usb_key_mapping += '{0:32} // {1} => {2}\n'.format(mapping, usb_key_name(key_info.name), key_info.c128_name)
        else:
            usb_key_mapping += '{false, nullptr, nullptr},\n'
    
    numlock_off_key_mapping = ''
    for key_code_num, key_info in numlock_off_keycodes.items():
        numlock_off_key_mapping += f'KeyInfo numlock_off_{key_info.name.lower()} {{true, &{key_info.matrix[0].name}, &{key_info.matrix[1].name}}};'
        numlock_off_key_mapping += f'     // {usb_key_name(key_info.name)} => {key_info.c128_name}\n'


    pins_set_array = ''
    for pin in Matrix:
        pins_set_array += f'    &{pin.name},'
        pins_set_array += '\n'

    with open(path.join(this_dir, '..', 'lib', 'KeyMapping', 'key_mapping.cpp'), 'w') as f:
        f.write(key_mapping_template.substitute(
            header_comment=header_comment,
            usb_key_mapping=usb_key_mapping,
            pins_set_array=pins_set_array,
            output_pins=output_pins,
            numlock_off_key_mapping=numlock_off_key_mapping
        ))

    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
