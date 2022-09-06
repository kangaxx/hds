//20220902 by gxx
//太泽项目业务代码
#pragma once
//新宇项目检测标准
class base_calc_std {
public:
	base_calc_std() {
		//
	}
};

class xinyu_calc_std:public base_calc_std {
public:
	xinyu_calc_std() {
		//wait to do
	}

	void set_value_w(double v, double v_a) {
		w = v;
		w_a = v_a;
	}

	void get_value_w(double& v, double& v_a) {
		v = w;
		v_a = w_a;
	}

	void set_value_l(double v, double v_a) {
		l = v;
		l_a = v_a;
	}

	void get_value_l(double& v, double& v_a) {
		v = l;
		v_a = l_a;
	}

	void set_value_w1(double v, double v_a) {
		w1 = v;
		w1_a = v_a;
	}

	void get_value_w1(double& v, double& v_a) {
		v = w1;
		v_a = w1_a;
	}

	void set_value_w2(double v, double v_a) {
		w2 = v;
		w2_a = v_a;
	}

	void get_value_w2(double& v, double& v_a) {
		v = w2;
		v_a = w2_a;
	}

	void set_value_h(double v, double v_a) {
		h = v;
		h_a = v_a;
	}

	void get_value_h(double& v, double& v_a) {
		v = h;
		v_a = h_a;
	}

	void set_value_lh(double v, double v_a) {
		lh = v;
		lh_a = v_a;
	}

	void get_value_lh(double& v, double& v_a) {
		v = lh;
		v_a = lh_a;
	}

	void set_value_h1(double v, double v_a) {
		h1 = v;
		h1_a = v_a;
	}

	void get_value_h1(double& v, double& v_a) {
		v = h1;
		v_a = h1_a;
	}

	void set_value_ra(double v[], double v_a[]) {
		for (int i = 0; sizeof(ra_a); ++i) {
			ra[i] = v[i];
			ra_a[i] = v_a[i];
		}
	}

	void get_value_ra(double* v, double* v_a) {
		for (int i = 0; sizeof(ra_a); ++i) {
			*v++ = ra[i];
			*v_a++ = ra_a[i];
		}
	}

	void set_value_rb(double v[], double v_a[]) {
		for (int i = 0; sizeof(rb_a); ++i) {
			rb[i] = v[i];
			rb_a[i] = v_a[i];
		}
	}

	void get_value_rb(double* v, double* v_a) {
		for (int i = 0; sizeof(ra_a); ++i) {
			*v++ = rb[i];
			*v_a++ = rb_a[i];
		}
	}
private:
	double w, w_a;
	double l, l_a;
	double lh, lh_a;
	double w1, w1_a;
	double w2, w2_a;
	double h, h_a;
	double h1, h1_a;
	double ra[2], ra_a[2];
	double rb[4], rb_a[4];
};
