#pragma once

//#define TMCDEBUG

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#endif

#include <Stream.h>
#include <SPI.h>
#include "source/SW_SPI.h"
#include "source/TMC2130_bitfields.h"
#include "source/TMC5130_bitfields.h"
#include "source/TMC5160_bitfields.h"
#include "source/TMC2208_bitfields.h"
#include "source/TMC2660_bitfields.h"

#define INIT_REGISTER(REG) REG##_t REG##_register = REG##_t
#define INIT2130_REGISTER(REG) TMC2130_n::REG##_t REG##_register = TMC2130_n::REG##_t
#define INIT5130_REGISTER(REG) TMC5130_n::REG##_t REG##_register = TMC5130_n::REG##_t
#define INIT5160_REGISTER(REG) TMC5160_n::REG##_t REG##_register = TMC5160_n::REG##_t
#define INIT2660_REGISTER(REG) TMC2660_n::REG##_t REG##_register = TMC2660_n::REG##_t
#define INIT2208_REGISTER(REG) TMC2208_n::REG##_t REG##_register = TMC2208_n::REG##_t
#define INIT2224_REGISTER(REG) TMC2224_n::REG##_t REG##_register = TMC2224_n::REG##_t
#define SET_ALIAS(TYPE, DRIVER, NEW, ARG, OLD) TYPE (DRIVER::*NEW)(ARG) = &DRIVER::OLD

#define TMCSTEPPER_VERSION 0x000001 // v0.0.1

class TMCStepper {
	public:
		virtual void rms_current(uint16_t mA);
		virtual void rms_current(uint16_t mA, float mult);
		virtual uint16_t rms_current();
		void hold_multiplier(float val) { holdMultiplier = val; }
		float hold_multiplier() { return holdMultiplier; }
		uint8_t test_connection();

		// Helper functions
		void microsteps(uint16_t ms);
		uint16_t microsteps();
		void blank_time(uint8_t value);
		uint8_t blank_time();
		void hysteresis_end(int8_t value);
		int8_t hysteresis_end();
		void hysteresis_start(uint8_t value);
		uint8_t hysteresis_start();

		// R+WC: GSTAT
		void 	GSTAT(							uint8_t input);
		uint8_t GSTAT();
		bool 	reset();
		bool 	drv_err();
		bool 	uv_cp();

		// W: IHOLD_IRUN
		void IHOLD_IRUN(					uint32_t input);
		uint32_t IHOLD_IRUN();
		void 	ihold(							uint8_t B);
		void 	irun(								uint8_t B);
		void 	iholddelay(					uint8_t B);
		uint8_t ihold();
		uint8_t irun();
		uint8_t iholddelay();

		// TPOWERDOWN
		uint8_t TPOWERDOWN();
		void TPOWERDOWN(					uint8_t input);

		// TSTEP
		uint32_t TSTEP();

		// TPWMTHRS
		uint32_t TPWMTHRS();
		void TPWMTHRS(						uint32_t input);

		// MSCNT
		uint16_t MSCNT();

		// R: MSCURACT
		uint32_t MSCURACT();
		int16_t cur_a();
		int16_t cur_b();

	protected:
		TMCStepper() {};
		TMCStepper(float RS) : Rsense(RS) {};
		INIT_REGISTER(GSTAT){0x01, {.sr=0}};
		INIT_REGISTER(IHOLD_IRUN){0x10, {.sr=0}};
		INIT_REGISTER(TPOWERDOWN){0x11, .sr=0};
		INIT_REGISTER(TSTEP){0x12};
		INIT_REGISTER(TPWMTHRS){0x13, .sr=0};
		INIT_REGISTER(MSCNT){0x6A};
		INIT_REGISTER(MSCURACT){0x6B, {.sr=0}};

		static constexpr uint8_t TMC_READ = 0x00,
														TMC_WRITE = 0x80;
		virtual void write(uint8_t, uint32_t) = 0;
		virtual uint32_t read(uint8_t) = 0;
		virtual void vsense(bool) = 0;
		virtual bool vsense(void) = 0;
		virtual bool otpw(void) = 0;
		virtual uint32_t DRV_STATUS() = 0;
		virtual void hend(uint8_t) = 0;
		virtual uint8_t hend() = 0;
		virtual void hstrt(uint8_t) = 0;
		virtual uint8_t hstrt() = 0;
		virtual void mres(uint8_t) = 0;
		virtual uint8_t mres() = 0;
		virtual void tbl(uint8_t) = 0;
		virtual uint8_t tbl() = 0;

		float Rsense;
		bool _started = 0;
		float holdMultiplier = 0.5;
};

class TMC2130Stepper : public TMCStepper {
	public:
		TMC2130Stepper(uint16_t pinCS, float RS);
		TMC2130Stepper(uint16_t pinCS, float RS, uint16_t pinMOSI, uint16_t pinMISO, uint16_t pinSCK);
		void begin();
		void setSPISpeed(uint32_t speed);
		void switchCSpin(bool state);
		//void checkStatus();
		bool isEnabled();
		void push();

		// Helper functions
		void sg_current_decrease(uint8_t value);
		uint8_t sg_current_decrease();

		// GCONF
		uint32_t GCONF();
		void GCONF(								uint32_t value);
		void I_scale_analog(			bool B);
		void internal_Rsense(			bool B);
		void en_pwm_mode(					bool B);
		void enc_commutation(			bool B);
		void shaft(								bool B);
		void diag0_error(					bool B);
		void diag0_otpw(					bool B);
		void diag0_stall(					bool B);
		void diag1_stall(					bool B);
		void diag1_index(					bool B);
		void diag1_onstate(				bool B);
		void diag1_steps_skipped(	bool B);
		void diag0_int_pushpull(	bool B);
		void diag1_pushpull(			bool B);
		void small_hysteresis(		bool B);
		void stop_enable(					bool B);
		void direct_mode(					bool B);
		bool I_scale_analog();
		bool internal_Rsense();
		bool en_pwm_mode();
		bool enc_commutation();
		bool shaft();
		bool diag0_error();
		bool diag0_otpw();
		bool diag0_stall();
		bool diag1_stall();
		bool diag1_index();
		bool diag1_onstate();
		bool diag1_steps_skipped();
		bool diag0_int_pushpull();
		bool diag1_pushpull();
		bool small_hysteresis();
		bool stop_enable();
		bool direct_mode();

		// IOIN
		uint32_t IOIN();
		bool step();
		bool dir();
		bool dcen_cfg4();
		bool dcin_cfg5();
		bool drv_enn_cfg6();
		bool dco();
		uint8_t version();

		// TCOOLTHRS
		uint32_t TCOOLTHRS();
		void TCOOLTHRS(						uint32_t input);

		// THIGH
		uint32_t THIGH();
		void THIGH(								uint32_t input);

		// XDRIRECT
		uint32_t XDIRECT();
		void XDIRECT(							uint32_t input);
		void coil_A(							int16_t 	B);
		void coil_B(							int16_t 	B);
		int16_t coil_A();
		int16_t coil_B();

		// VDCMIN
		uint32_t VDCMIN();
		void VDCMIN(							uint32_t input);

		// CHOPCONF
		uint32_t CHOPCONF();
		void CHOPCONF(						uint32_t value);
		void toff(								uint8_t B);
		void hstrt(								uint8_t B);
		void hend(								uint8_t B);
		//void fd(									uint8_t B);
		void disfdcc(							bool 		B);
		void rndtf(								bool 		B);
		void chm(									bool 		B);
		void tbl(									uint8_t B);
		void vsense(							bool 		B);
		void vhighfs(							bool 		B);
		void vhighchm(						bool 		B);
		void sync(								uint8_t B);
		void mres(								uint8_t B);
		void intpol(							bool 		B);
		void dedge(								bool 		B);
		void diss2g(							bool 		B);
		uint8_t toff();
		uint8_t hstrt();
		uint8_t hend();
		//uint8_t fd();
		bool 	disfdcc();
		bool 	rndtf();
		bool 	chm();
		uint8_t tbl();
		bool 	vsense();
		bool 	vhighfs();
		bool 	vhighchm();
		uint8_t sync();
		uint8_t mres();
		bool 	intpol();
		bool 	dedge();
		bool 	diss2g();

		// COOLCONF
		void COOLCONF(uint32_t value);
		uint32_t COOLCONF();
		void semin(								uint8_t B);
		void seup(								uint8_t B);
		void semax(								uint8_t B);
		void sedn(								uint8_t B);
		void seimin(							bool 		B);
		void sgt(									int8_t  B);
		void sfilt(								bool 		B);
		uint8_t semin();
		uint8_t seup();
		uint8_t semax();
		uint8_t sedn();
		bool seimin();
		int8_t sgt();
		bool sfilt();

		// DRV_STATUS
		uint32_t DRV_STATUS();
		uint16_t sg_result();
		bool fsactive();
		uint8_t cs_actual();
		bool stallguard();
		bool ot();
		bool otpw();
		bool s2ga();
		bool s2gb();
		bool ola();
		bool olb();
		bool stst();

		// PWMCONF
		void PWMCONF(							uint32_t value);
		uint32_t PWMCONF();
		void pwm_ampl(						uint8_t B);
		void pwm_grad(						uint8_t B);
		void pwm_freq(						uint8_t B);
		void pwm_autoscale(				bool		B);
		void pwm_symmetric(				bool		B);
		void freewheel(						uint8_t B);
		uint8_t pwm_ampl();
		uint8_t pwm_grad();
		uint8_t pwm_freq();
		bool 	pwm_autoscale();
		bool 	pwm_symmetric();
		uint8_t freewheel();

		// PWM_SCALE
		uint8_t PWM_SCALE();
		
		// ENCM_CTRL
		uint8_t ENCM_CTRL();
		void ENCM_CTRL(						uint8_t input);
		void inv(									bool B);
		void maxspeed(						bool B);
		bool inv();
		bool maxspeed();
		
		// LOST_STEPS
		uint32_t LOST_STEPS();

		// Function aliases

		//protected:
		void write(uint8_t addressByte, uint32_t config);
		uint32_t read(uint8_t addressByte);

		INIT_REGISTER(GCONF){0x00, {.sr=0}};
		INIT_REGISTER(IOIN){0x04, {.sr=0}};
		INIT_REGISTER(TCOOLTHRS){0x14, .sr=0};
		INIT_REGISTER(THIGH){0x15, .sr=0};
		INIT_REGISTER(XDIRECT){0x2D, {.sr=0}};
		INIT_REGISTER(VDCMIN){0x33, .sr=0};
		INIT_REGISTER(CHOPCONF){0x6C, {.sr=0}};
		INIT_REGISTER(COOLCONF){0x6D, {.sr=0}};
		INIT_REGISTER(DRV_STATUS){0x6F, {.sr=0}};
		INIT_REGISTER(PWMCONF){0x70, {.sr=0}};
		INIT_REGISTER(PWM_SCALE){0x71};
		INIT_REGISTER(ENCM_CTRL){0x72, {.sr=0}};
		INIT_REGISTER(LOST_STEPS){0x73};

		uint8_t status_response;
		uint32_t spi_speed = 16000000/8; // Default 2MHz
		const uint16_t _pinCS;
		const bool uses_sw_spi = false;
};

class TMC5130Stepper : public TMC2130Stepper {
	public:
		TMC5130Stepper(uint16_t pinCS);
		TMC5130Stepper(uint16_t pinCS, float RS);
		void begin();

		// R: IFCNT
		uint8_t IFCNT();
		// W: SLAVECONF
		uint16_t SLAVECONF();
		void SLAVECONF(uint16_t input);
		// IOIN
		uint32_t 	IOIN();
		bool 			refl_step();
		bool 			refr_dir();
		bool 			encb_dcen_cfg4();
		bool 			enca_dcin_cfg5();
		bool 			drv_enn_cfg6();
		bool 			enc_n_dco();
		bool 			sd_mode();
		bool 			swcomp_in();
		uint8_t 	version();

		// GCONF
		void diag1_poscomp_pushpull(bool B) { diag1_pushpull(B); }
		bool diag1_poscomp_pushpull() { return diag1_pushpull(); }

		// SW_MODE
		uint32_t SW_MODE();
		void SW_MODE(uint32_t input);

		void stop_l_enable(bool B);
		void stop_r_enable(bool B);
		void pol_stop_l(bool B);
		void pol_stop_r(bool B);
		void swap_lr(bool B);
		void latch_l_active(bool B);
		void latch_l_inactive(bool B);
		void latch_r_active(bool B);
		void latch_r_inactive(bool B);
		void en_latch_encoder(bool B);
		void sg_stop(bool B);
		void en_softstop(bool B);

		bool stop_r_enable();
		bool pol_stop_l();
		bool pol_stop_r();
		bool swap_lr();
		bool latch_l_active();
		bool latch_l_inactive();
		bool latch_r_active();
		bool latch_r_inactive();
		bool en_latch_encoder();
		bool sg_stop();
		bool en_softstop();

		// RAMP_STAT
		uint32_t RAMP_STAT();
		bool status_stop_l();
		bool status_stop_r();
		bool status_latch_l();
		bool status_latch_r();
		bool event_stop_l();
		bool event_stop_r();
		bool event_stop_sg();
		bool event_pos_reached();
		bool velocity_reached();
		bool position_reached();
		bool vzero();
		bool t_zerowait_active();
		bool second_move();
		bool status_sg();

		// ENCMODE
		uint32_t ENCMODE();
		void ENCMODE(uint32_t input);
		void pol_a(bool B);
		void pol_b(bool B);
		void pol_n(bool B);
		void ignore_ab(bool B);
		void clr_cont(bool B);
		void clr_once(bool B);
		void pos_edge(bool B);
		void neg_edge(bool B);
		void clr_enc_x(bool B);
		void latch_x_act(bool B);
		void enc_sel_decimal(bool B);
		bool pol_a();
		bool pol_b();
		bool pol_n();
		bool ignore_ab();
		bool clr_cont();
		bool clr_once();
		bool pos_edge();
		bool neg_edge();
		bool clr_enc_x();
		bool latch_x_act();
		bool enc_sel_decimal();

		// W: OUTPUT
		bool TMC_OUTPUT();
		void TMC_OUTPUT(bool input);
		// W: X_COMPARE
		uint32_t X_COMPARE();
		void X_COMPARE(uint32_t input);
		// RW: RAMPMODE
		uint8_t RAMPMODE();
		void RAMPMODE(uint8_t input);
		// RW: XACTUAL
		int32_t XACTUAL();
		void XACTUAL(int32_t input);
		// R: VACTUAL
		int32_t VACTUAL();
		// W: VSTART
		uint32_t VSTART();
		void VSTART(uint32_t input);
		// W: A1
		uint16_t A1();
		void A1(uint16_t input);
		// W: V1
		uint32_t V1();
		void V1(uint32_t input);
		// W: AMAX
		uint16_t AMAX();
		void AMAX(uint16_t input);
		// W: VMAX
		uint32_t VMAX();
		void VMAX(uint32_t input);
		// W: DMAX
		uint16_t DMAX();
		void DMAX(uint16_t input);
		// W: D1
		uint16_t D1();
		void D1(uint16_t input);
		// W: VSTOP
		uint32_t VSTOP();
		void VSTOP(uint32_t input);
		// W: TZEROWAIT
		uint16_t TZEROWAIT();
		void TZEROWAIT(uint16_t input);
		// RW: XTARGET
		int32_t XTARGET();
		void XTARGET(int32_t input);

		INIT_REGISTER(IFCNT){0x02};
		INIT_REGISTER(SLAVECONF){0x03, {.sr=0}};
		INIT5130_REGISTER(IOIN){0x04, {.sr=0}};
		INIT_REGISTER(OUTPUT){0x04, .sr=0};
		INIT_REGISTER(X_COMPARE){0x05, .sr=0};
		INIT_REGISTER(RAMPMODE){0x20, .sr=0};
		INIT_REGISTER(XACTUAL){0x21, .sr=0};
		INIT_REGISTER(VACTUAL){0x22};
		INIT_REGISTER(VSTART){0x23, .sr=0};
		INIT_REGISTER(A1){0x24, .sr=0};
		INIT_REGISTER(V1){0x25, .sr=0};
		INIT_REGISTER(AMAX){0x26, .sr=0};
		INIT_REGISTER(VMAX){0x27, .sr=0};
		INIT_REGISTER(DMAX){0x28, .sr=0};
		INIT_REGISTER(D1){0x2A, .sr=0};
		INIT_REGISTER(VSTOP){0x2B, .sr=0};
		INIT_REGISTER(TZEROWAIT){0x2C, .sr=0};
		INIT_REGISTER(XTARGET){0x2D};
		INIT_REGISTER(SW_MODE){0x34, {.sr=0}};
		INIT_REGISTER(RAMP_STAT){0x35, {.sr=0}};
		INIT_REGISTER(XLATCH){0x36};
		INIT_REGISTER(ENCMODE){0x38, {.sr=0}};
		INIT_REGISTER(X_ENC){0x39};
		INIT_REGISTER(ENC_CONST){0x3A, .sr=0};
		INIT_REGISTER(ENC_STATUS){0x3B};
		INIT_REGISTER(ENC_LATCH){0x3C};
		/*
		INIT_REGISTER(MSLUT0){0x60, 0};
		INIT_REGISTER(MSLUT1){0x61, 0};
		INIT_REGISTER(MSLUT2){0x62, 0};
		INIT_REGISTER(MSLUT3){0x63, 0};
		INIT_REGISTER(MSLUT4){0x64, 0};
		INIT_REGISTER(MSLUT5){0x65, 0};
		INIT_REGISTER(MSLUT6){0x66, 0};
		INIT_REGISTER(MSLUT7){0x67, 0};
		INIT_REGISTER(MSLUTSEL){0x68, {0,0,0,0,0,0,0}};
		INIT_REGISTER(MSLUTSTART){0x69, {0,0}};
		INIT_REGISTER(MSCNT){0x6A};
		INIT_REGISTER(MSCURACT){0x6B, {0,0}};
		INIT_REGISTER(DCCTRL){0x6E, {0,0}};
		*/
};

class TMC5160Stepper : public TMC5130Stepper {
	public:
		TMC5160Stepper(uint16_t pinCS, float RS);

		void rms_current(uint16_t mA);
		void rms_current(uint16_t mA, float mult);
		uint16_t rms_current();

		// RW: GCONF
		void recalibrate(bool);
		void faststandstill(bool);
		void multistep_filt(bool);
		bool recalibrate();
		bool faststandstill();
		bool multistep_filt();

		// R: IOIN
		bool drv_enn() { return drv_enn_cfg6(); }
		bool enc_n_dco_cfg6() { return enc_n_dco(); }

		// W: OTP_PROG
		// R: OTP_READ
		// RW: FACTORY_CONF

		// W: SHORT_CONF
		void SHORT_CONF(uint32_t);
		void s2vs_level(uint8_t);
		void s2g_level(uint8_t);
		void shortfilter(uint8_t);
		void shortdelay(bool);
		uint32_t SHORT_CONF();
		uint8_t s2vs_level();
		uint8_t s2g_level();
		uint8_t shortfilter();
		bool shortdelay();

		// W: DRV_CONF
		void DRV_CONF(uint32_t);
		void bbmtime(uint8_t);
		void bbmclks(uint8_t);
		void otselect(uint8_t);
		void drvstrength(uint8_t);
		void filt_isense(uint8_t);
		uint32_t DRV_CONF();
		uint8_t bbmtime();
		uint8_t bbmclks();
		uint8_t otselect();
		uint8_t drvstrength();
		uint8_t filt_isense();

		// W: GLOBAL_SCALER
		void GLOBAL_SCALER(uint8_t);
		uint8_t GLOBAL_SCALER();

		// R: OFFSET_READ
		uint16_t OFFSET_READ();

		// R+WC: ENC_STATUS
		void ENC_STATUS(uint8_t);
		uint8_t ENC_STATUS();

		// W: ENC_DEVIATION
		void ENC_DEVIATION(uint32_t);
		uint32_t ENC_DEVIATION();

		// R: PWM_SCALE
		uint32_t PWM_SCALE();
		uint8_t pwm_scale_sum();
		uint16_t pwm_scale_auto();

		// R: PWM_AUTO
		uint32_t PWM_AUTO();
		uint8_t pwm_ofs_auto();
		uint8_t pwm_grad_auto();

		// RW: CHOPCONF
		void diss2vs(bool);
		void tpfd(uint8_t);
		bool diss2vs();
		uint8_t tpfd();

		// W: PWM_CONF
		void PWM_CONF(uint32_t);
		void pwm_lim(uint8_t);
		void pwm_reg(uint8_t);
		void pwm_autograd(bool);
		uint32_t PWM_CONF();
		uint8_t pwm_lim();
		uint8_t pwm_reg();
		bool pwm_autograd();

	private:
		using TMC5130Stepper::I_scale_analog;
		using TMC5130Stepper::internal_Rsense;
		using TMC5130Stepper::enc_commutation;
		using TMC5130Stepper::drv_enn_cfg6;
		using TMC5130Stepper::enc_n_dco;
		using TMC5130Stepper::ENCM_CTRL;
		using TMC5130Stepper::vsense;
		using TMC5130Stepper::rndtf;

		INIT_REGISTER(DRV_CONF){0x0A, {.sr=0}};
		INIT_REGISTER(SHORT_CONF){0x09, {.sr=0}};
		INIT_REGISTER(GLOBAL_SCALER){0x0B, .sr=0};
		INIT_REGISTER(OFFSET_READ){0x0C, .sr=0};
		INIT_REGISTER(ENC_DEVIATION){0x3D, .sr=0};
		//INIT_REGISTER(PWM_SCALE){0x0C, .sr=0};
		INIT_REGISTER(PWM_AUTO){0x72, {.sr=0}};
		INIT5160_REGISTER(PWMCONF){0x70, {.sr=0}};
};

class TMC2208Stepper : public TMCStepper {
	public:
		//TMC2208Stepper(HardwareSerial& serial);
		TMC2208Stepper(Stream * SerialPort, float RS, bool has_rx=true);
		TMC2208Stepper(uint16_t SW_RX_pin, uint16_t SW_TX_pin, float RS, bool has_rx=true);
		void push();
		void beginSerial(uint32_t baudrate);
		bool isEnabled();

		// RW: GCONF
		void GCONF(uint32_t input);
		void I_scale_analog(bool B);
		void internal_Rsense(bool B);
		void en_spreadCycle(bool B);
		void shaft(bool B);
		void index_otpw(bool B);
		void index_step(bool B);
		void pdn_disable(bool B);
		void mstep_reg_select(bool B);
		void multistep_filt(bool B);
		uint32_t GCONF();
		bool I_scale_analog();
		bool internal_Rsense();
		bool en_spreadCycle();
		bool shaft();
		bool index_otpw();
		bool index_step();
		bool pdn_disable();
		bool mstep_reg_select();
		bool multistep_filt();

		// R: IFCNT
		uint8_t IFCNT();

		// W: SLAVECONF
		void SLAVECONF(uint16_t input);
		uint16_t SLAVECONF();
		void senddelay(uint8_t B);
		uint8_t senddelay();

		// W: OTP_PROG
		void OTP_PROG(uint16_t input);

		// R: OTP_READ
		uint32_t OTP_READ();

		// R: IOIN
		uint32_t IOIN();
		bool enn();
		bool ms1();
		bool ms2();
		bool diag();
		bool pdn_uart();
		bool step();
		bool sel_a();
		bool dir();
		uint8_t version();

		// RW: FACTORY_CONF
		void FACTORY_CONF(uint16_t input);
		uint16_t FACTORY_CONF();
		void fclktrim(uint8_t B);
		void ottrim(uint8_t B);
		uint8_t fclktrim();
		uint8_t ottrim();

		// W: VACTUAL
		void VACTUAL(uint32_t input);
		uint32_t VACTUAL();

		// RW: CHOPCONF
		void CHOPCONF(uint32_t input);
		void toff(uint8_t B);
		void hstrt(uint8_t B);
		void hend(uint8_t B);
		void tbl(uint8_t B);
		void vsense(bool B);
		void mres(uint8_t B);
		void intpol(bool B);
		void dedge(bool B);
		void diss2g(bool B);
		void diss2vs(bool B);
		uint32_t CHOPCONF();
		uint8_t toff();
		uint8_t hstrt();
		uint8_t hend();
		uint8_t tbl();
		bool vsense();
		uint8_t mres();
		bool intpol();
		bool dedge();
		bool diss2g();
		bool diss2vs();

		// R: DRV_STATUS
		uint32_t DRV_STATUS();
		bool otpw();
		bool ot();
		bool s2ga();
		bool s2gb();
		bool s2vsa();
		bool s2vsb();
		bool ola();
		bool olb();
		bool t120();
		bool t143();
		bool t150();
		bool t157();
		uint16_t cs_actual();
		bool stealth();
		bool stst();

		// RW: PWMCONF
		void PWMCONF(uint32_t input);
		void pwm_ofs(uint8_t B);
		void pwm_grad(uint8_t B);
		void pwm_freq(uint8_t B);
		void pwm_autoscale(bool B);
		void pwm_autograd(bool B);
		void freewheel(uint8_t B);
		void pwm_reg(uint8_t B);
		void pwm_lim(uint8_t B);
		uint32_t PWMCONF();
		uint8_t pwm_ofs();
		uint8_t pwm_grad();
		uint8_t pwm_freq();
		bool pwm_autoscale();
		bool pwm_autograd();
		uint8_t freewheel();
		uint8_t pwm_reg();
		uint8_t pwm_lim();

		// R: PWM_SCALE
		uint32_t PWM_SCALE();
		uint8_t pwm_scale_sum();
		int16_t pwm_scale_auto();

		bool isWriteOnly() {return write_only;}

		uint16_t bytesWritten = 0;
		float Rsense = 0.11;
		uint16_t replyDelay = 10;
		bool CRCerror = false;
	protected:
		//INIT_REGISTER(GCONF)					{0x00, {.sr=0}};
		INIT2208_REGISTER(GCONF)			{0x00, {.sr=0}};
		INIT_REGISTER(IFCNT)					{0x02};
		INIT_REGISTER(SLAVECONF)			{0x03, {.sr=0}};
		INIT_REGISTER(OTP_PROG)				{0x04};
		INIT_REGISTER(OTP_READ)				{0x05};
		//INIT_REGISTER(IOIN_2208)			{0x06, {.sr=0}};
		INIT2208_REGISTER(IOIN)				{0x06, {.sr=0}};
		INIT_REGISTER(FACTORY_CONF)		{0x07, {.sr=0}};
		//INIT_REGISTER(VACTUAL_2208)		{0x22, 0};
		INIT2208_REGISTER(VACTUAL)		{0x22, 0};
		//INIT_REGISTER(CHOPCONF_2208)	{0x6C, {.sr=0}};
		INIT2208_REGISTER(CHOPCONF){0x6C, {.sr=0}};
		//INIT_REGISTER(DRV_STATUS_2208){0x6F, {.sr=0}};
		INIT2208_REGISTER(DRV_STATUS)	{0x6F, {.sr=0}};
		//INIT_REGISTER(PWMCONF_2208)		{0x70, {.sr=0}};
		INIT2208_REGISTER(PWMCONF)		{0x70, {.sr=0}};
		//INIT_REGISTER(PWM_SCALE_2208)	{0x71, {.sr=0}};
		INIT2208_REGISTER(PWM_SCALE)	{0x71, {.sr=0}};
		INIT_REGISTER(PWM_AUTO)				{0x72, {.sr=0}};

		void * SerialObject;
		void write(uint8_t, uint32_t);
		uint32_t read(uint8_t);
		uint8_t calcCRC(uint8_t datagram[], uint8_t len);
		static constexpr uint8_t  TMC2208_SYNC = 0x05,
															TMC2208_SLAVE_ADDR = 0x00;
		bool write_only;
		bool uses_sw_serial;
		uint16_t mA_val = 0;
};

class TMC2224Stepper : public TMC2208Stepper {
	public:
		uint32_t IOIN();
		bool enn();
		bool ms1();
		bool ms2();
		bool pdn_uart();
		bool spread();
		bool step();
		bool sel_a();
		bool dir();
		uint8_t version();
	protected:
		IOIN_2224_t IOIN_register = IOIN_2224_t{0x06, {.sr=0}};

};

class TMC2660Stepper {
	public:
		TMC2660Stepper(uint16_t pinCS, float RS);
		TMC2660Stepper(uint16_t pinCS, float RS, uint16_t pinMOSI, uint16_t pinMISO, uint16_t pinSCK);
		void write(uint8_t addressByte, uint32_t config);
		uint32_t read();
		void switchCSpin(bool state);
		void begin();
		bool isEnabled();
		uint8_t test_connection();
		uint16_t rms_current();
		void rms_current(uint16_t mA);
		//uint16_t getMilliamps() {return val_mA;}
		void push();
		uint8_t savedToff() { return _savedToff; }

		// Helper functions
		void microsteps(uint16_t ms);
		uint16_t microsteps();
		void blank_time(uint8_t value);
		uint8_t blank_time();
		void hysteresis_end(int8_t value);
		int8_t hysteresis_end();
		void hysteresis_start(uint8_t value);
		uint8_t hysteresis_start();

		// W: DRVCONF
		void DRVCONF(uint32_t);
		void tst(bool);
		void slph(uint8_t);
		void slpl(uint8_t);
		void diss2g(bool);
		void ts2g(uint8_t);
		void sdoff(bool);
		void vsense(bool);
		void rdsel(uint8_t);
		uint32_t DRVCONF();
		bool tst();
		uint8_t slph();
		uint8_t slpl();
		bool diss2g();
		uint8_t ts2g();
		bool sdoff();
		bool vsense();
		uint8_t rdsel();

		// W: DRVCTRL
		void DRVCTRL(uint32_t);
		void pha(bool B);
		void ca(uint8_t B);
		void phb(bool B);
		void cb(uint8_t B);
		bool pha();
		uint8_t ca();
		bool phb();
		uint8_t cb();
		void intpol(bool);
		void dedge(bool);
		void mres(uint8_t);
		uint32_t DRVCTRL();
		bool intpol();
		bool dedge();
		uint8_t mres();

		// W: CHOPCONF
		void CHOPCONF(uint32_t);
		void tbl(uint8_t);
		void chm(bool);
		void rndtf(bool);
		void hdec(uint8_t);
		void hend(uint8_t);
		void hstrt(uint8_t);
		void toff(uint8_t);
		uint32_t CHOPCONF();
		uint8_t tbl();
		bool chm();
		bool rndtf();
		uint8_t hdec();
		uint8_t hend();
		uint8_t hstrt();
		uint8_t toff();

		// R: DRVSTATUS
		uint32_t DRV_STATUS() { return DRVSTATUS(); }
		uint32_t DRVSTATUS();
		uint16_t mstep();
		uint8_t se();
		bool stst();
		bool olb();
		bool ola();
		bool s2gb();
		bool s2ga();
		bool otpw();
		bool ot();
		bool sg();
		uint16_t sg_result();

		// W: SGCSCONF
		uint32_t SGCSCONF();
		void sfilt(bool);
		void sgt(uint8_t);
		void cs(uint8_t);
		void SGCSCONF(uint32_t);
		bool sfilt();
		uint8_t sgt();
		uint8_t cs();

		// W: SMARTEN
		void SMARTEN(uint32_t);
		void seimin(bool B);
		void sedn(uint8_t B);
		void semax(uint8_t B);
		void seup(uint8_t B);
		void semin(uint8_t B);
		uint32_t SMARTEN();
		bool seimin();
		uint8_t sedn();
		uint8_t semax();
		uint8_t seup();
		uint8_t semin();
		/*
		// Alias
		SET_ALIAS(void, polarity_A, bool, pha);
		SET_ALIAS(void, current_A, uint8_t, ca);
		SET_ALIAS(void, polarity_B, bool, phb);
		SET_ALIAS(void, current_b, uint8_t, cb);
		SET_ALIAS(void, interpolate, bool, intpol);
		SET_ALIAS(void, double_edge_step, bool, dedge);
		SET_ALIAS(void, microsteps, uint8_t, mres);
		SET_ALIAS(void, blank_time, uint8_t, tbl);
		SET_ALIAS(void, chopper_mode, bool, chm);
		SET_ALIAS(void, random_off_time, bool, rndtf);
		SET_ALIAS(void, hysteresis_decrement, uint8_t, hdec);
		SET_ALIAS(void, hysteresis_low, uint8_t, hend);
		SET_ALIAS(void, hysteresis_start, uint8_t, hstrt);
		SET_ALIAS(void, off_time, uint8_t, toff);
		*/
		INIT_REGISTER(DRVCTRL_1){0b00, {.sr=0}};
		INIT_REGISTER(DRVCTRL_0){0b00, {.sr=0}};
		INIT2660_REGISTER(CHOPCONF){0b100, {.sr=0}};
		INIT_REGISTER(SMARTEN){0b101, {.sr=0}};
		INIT_REGISTER(SGCSCONF){0b110, {.sr=0}};
		INIT_REGISTER(DRVCONF){0b111, {.sr=0}};
		INIT_REGISTER(READ_RDSEL00){{.sr=0}};
		INIT_REGISTER(READ_RDSEL01){{.sr=0}};
		INIT_REGISTER(READ_RDSEL10){{.sr=0}};

		uint16_t _pinCS;
		float Rsense;
		bool _started;
		uint8_t status_response;
		//uint16_t val_mA           = 0;
		float holdMultiplier = 0.5;
		uint32_t spi_speed = 16000000/8; // Default 2MHz
		bool uses_sw_spi = false;
		uint8_t _savedToff = 0;
};
