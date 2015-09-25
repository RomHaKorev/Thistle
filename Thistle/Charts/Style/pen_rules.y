pen_prop
: PEN ':' pen_value ';'
| P_PENSTYLE ':' value_pen_style ';'
| P_PENCOLOR ':' brush ';'
;

pen_value
: pen_color
| brush pen_size
| brush pen_size value_pen_style
| brush pen_size value_pen_style value_pen_capstyle
| brush pen_size value_pen_style value_pen_capstyle value_pen_joinstyle
;

pen_size
: INTEGER UNIT { SerieFormatDigester::instance->currentPen().setWidth( yylval ); }
;

value_pen_style
: V_PEN_STYLE { SerieFormatDigester::instance->currentPen().setStyle( static_cast<Qt::PenStyle>(yylval) ); }
;

value_pen_capstyle
: V_PEN_CAPSTYLE { SerieFormatDigester::instance->currentPen().setCapStyle( static_cast<Qt::PenCapStyle>(yylval) ); }
;

value_pen_joinstyle
: V_PEN_JOINSTYLE { SerieFormatDigester::instance->currentPen().setJoinStyle( static_cast<Qt::PenJoinStyle>(yylval) ); }
;

pen_color
: brush { SerieFormatDigester::instance->currentPen().setColor( yylval ); }
;