#include "TMCStepper.h"
#include "TMC_MACROS.h"

#define SET_REG(SETTING) ENCMODE_register.cfg.opt.SETTING = B; WRITE_REG(ENCMODE);
#define GET_REG(SETTING) ENCMODE(); return ENCMODE_register.cfg.opt.SETTING;

// ENCMODE
uint32_t TMC5130Stepper::ENCMODE() {
	ENCMODE_register.cfg.sr = READ_REG(ENCMODE);
	return ENCMODE_register.cfg.sr;
}
void TMC5130Stepper::ENCMODE(uint32_t input) {
	ENCMODE_register.cfg.sr = input;
	WRITE_REG(ENCMODE);
}

void TMC5130Stepper::pol_a(bool B)			{ SET_REG(pol_a);			}
void TMC5130Stepper::pol_b(bool B)			{ SET_REG(pol_b);			}
void TMC5130Stepper::pol_n(bool B)			{ SET_REG(pol_n);			}
void TMC5130Stepper::ignore_ab(bool B)		{ SET_REG(ignore_ab);		}
void TMC5130Stepper::clr_cont(bool B)		{ SET_REG(clr_cont);		}
void TMC5130Stepper::clr_once(bool B)		{ SET_REG(clr_once);		}
void TMC5130Stepper::pos_edge(bool B)		{ SET_REG(pos_edge);		}
void TMC5130Stepper::neg_edge(bool B)		{ SET_REG(neg_edge);		}
void TMC5130Stepper::clr_enc_x(bool B)		{ SET_REG(clr_enc_x);		}
void TMC5130Stepper::latch_x_act(bool B)	{ SET_REG(latch_x_act);		}
void TMC5130Stepper::enc_sel_decimal(bool B){ SET_REG(enc_sel_decimal);	}

bool TMC5130Stepper::pol_a()			{ GET_REG(pol_a);			}
bool TMC5130Stepper::pol_b()			{ GET_REG(pol_b);			}
bool TMC5130Stepper::pol_n()			{ GET_REG(pol_n);			}
bool TMC5130Stepper::ignore_ab()		{ GET_REG(ignore_ab);		}
bool TMC5130Stepper::clr_cont()			{ GET_REG(clr_cont);		}
bool TMC5130Stepper::clr_once()			{ GET_REG(clr_once);		}
bool TMC5130Stepper::pos_edge()			{ GET_REG(pos_edge);		}
bool TMC5130Stepper::neg_edge()			{ GET_REG(neg_edge);		}
bool TMC5130Stepper::clr_enc_x()		{ GET_REG(clr_enc_x);		}
bool TMC5130Stepper::latch_x_act()		{ GET_REG(latch_x_act);		}
bool TMC5130Stepper::enc_sel_decimal()	{ GET_REG(enc_sel_decimal);	}