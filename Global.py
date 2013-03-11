def enum(**enums):
    return type( 'Enum', (), enums)

Color = enum( 
             Blue       = 0x6090e4,
             Green      = 0xc9ff71,
             Orange     = 0xff9441,
             Purple     = 0x9c78c0,
             Red        = 0xff7474,
             Yellow     = 0xfff76d,
             Gray       = 0x868686,
             White      = 0xFFFFFF,

             LightBlue   = 0x4889F1,
             LightGreen  = 0x9AEC7A,
             LightOrange = 0xFC9834,
             LightPurple = 0x8274B0,
             LightRed    = 0xFC5656,
             LightYellow = 0xFFF46A,
             LightGray   = 0xBEBEBE,

             DarkBlue   = 0x0839A1,
             DarkGray   = 0x505050,
             DarkGreen  = 0x8CD01E,
             DarkOrange = 0xE46B0F,
             DarkPurple = 0x6D3AA0,
             DarkYellow = 0xFFF219
            )

Shape = enum(
            Rectangle = 0,
            Ellipse   = 1,
            RoundedRect = 2,
            Diamond = 3,
            Triangle = 4,
            ReversedTriangle = 5
           )