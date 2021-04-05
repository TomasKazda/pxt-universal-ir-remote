// tests go here; this will not be compiled when this package is used as an extension.
basic.showLeds(`
. . . . .
. . . . .
. . # . .
. . . . .
. . . . .
`)
IR.onIR_received(function (code: number) {
    switch (code) {
        case 0xFF00:break;
        case 57630:
            serial.writeLine("code: OK")
            break;
        default:
            serial.writeLine("code: " + IR.IR_readHex())
    }
})
