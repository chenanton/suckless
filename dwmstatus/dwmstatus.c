/*
 * Copy me if you can.
 * by 20h
 * RUN COMMAND: sudo make PREFIX=/usr install
 */

#define _BSD_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <strings.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <X11/Xlib.h>

char *tzargentina = "America/Buenos_Aires";
char *tzutc = "UTC";
char *tzberlin = "Europe/Berlin";
char *tzcalgary = "Canada/Mountain";
char *tzvancouver = "Canada/Pacific";

static Display *dpy;

	char *
smprintf(char *fmt, ...)
{
	va_list fmtargs;
	char *ret;
	int len;

	va_start(fmtargs, fmt);
	len = vsnprintf(NULL, 0, fmt, fmtargs);
	va_end(fmtargs);

	ret = malloc(++len);
	if (ret == NULL) {
		perror("malloc");
		exit(1);
	}

	va_start(fmtargs, fmt);
	vsnprintf(ret, len, fmt, fmtargs);
	va_end(fmtargs);

	return ret;
}

	void
settz(char *tzname)
{
	setenv("TZ", tzname, 1);
}

	char *
mktimes(char *fmt, char *tzname)
{
	char buf[129];
	time_t tim;
	struct tm *timtm;

	settz(tzname);
	tim = time(NULL);
	timtm = localtime(&tim);
	if (timtm == NULL)
		return smprintf("");

	if (!strftime(buf, sizeof(buf)-1, fmt, timtm)) {
		fprintf(stderr, "strftime == 0\n");
		return smprintf("");
	}

	return smprintf("%s", buf);
}

	void
setstatus(char *str)
{
	XStoreName(dpy, DefaultRootWindow(dpy), str);
	XSync(dpy, False);
}

	char *
loadavg(void)
{
	double avgs[3];

	if (getloadavg(avgs, 3) < 0)
		return smprintf("");

	return smprintf("%.2f %.2f %.2f", avgs[0], avgs[1], avgs[2]);
}

	char *
readfile(char *base, char *file)
{
	char *path, line[513];
	FILE *fd;

	memset(line, 0, sizeof(line));

	path = smprintf("%s/%s", base, file);
	fd = fopen(path, "r");
	free(path);
	if (fd == NULL)
		return NULL;

	if (fgets(line, sizeof(line)-1, fd) == NULL)
		return NULL;
	fclose(fd);

	return smprintf("%s", line);
}

	char *
getbattery(char *base)
{
	char *co, *status;
	int descap, remcap;

	descap = -1;
	remcap = -1;

	co = readfile(base, "present");
	if (co == NULL)
		return smprintf("");
	if (co[0] != '1') {
		free(co);
		return smprintf("not present");
	}
	free(co);

	co = readfile(base, "charge_full_design");
	if (co == NULL) {
		co = readfile(base, "energy_full_design");
		if (co == NULL)
			return smprintf("");
	}
	sscanf(co, "%d", &descap);
	free(co);

	co = readfile(base, "charge_now");
	if (co == NULL) {
		co = readfile(base, "energy_now");
		if (co == NULL)
			return smprintf("");
	}
	sscanf(co, "%d", &remcap);
	free(co);

	float batpercent = ((float) remcap / (float) descap) * 100;

	co = readfile(base, "status");
	if (!strncmp(co, "Discharging", 11)) {
		if (batpercent > 87.5) status = "";
		else if (batpercent > 62.5) status = "";
		else if (batpercent > 37.5) status = "";
		else if (batpercent > 12.5) status = "";
		else status = "";
	} else if(!strncmp(co, "Charging", 8)) {
		status = "";
	} else {
		status = "?";
	}

	if (remcap < 0 || descap < 0)
		return smprintf("invalid");

	// return smprintf("%s %.0f%%", status, batpercent);
	return smprintf("%.0f%%", batpercent);
}

	char *
gettemperature(char *base, char *sensor)
{
	char *co;

	co = readfile(base, sensor);
	if (co == NULL)
		return smprintf("");
	return smprintf("%02.0f°C", atof(co) / 1000);
}


	int
main(void)
{
	char *status;
	char *avgs;
	char *bat;
	char *tmar;
	char *tmutc;
	char *tmbln;
	char *time_yyc;
	char *date_yyc;
	char *t0, *t1, *t2;

	if (!(dpy = XOpenDisplay(NULL))) {
		fprintf(stderr, "dwmstatus: cannot open display.\n");
		return 1;
	}

	for (;;sleep(60)) {
		avgs = loadavg();
		// bat = getbattery("/sys/class/power_supply/AC/BAT0");
		tmar = mktimes("%H:%M", tzargentina);
		tmutc = mktimes("%H:%M", tzutc);
		tmbln = mktimes("KW %W %a %d %b %H:%M %Z %Y", tzberlin);
		time_yyc = mktimes(" %I:%M %p", tzcalgary);
		// time_yyc = mktimes("%I:%M %p", tzcalgary);
		date_yyc = mktimes(" %m/%d/%Y", tzcalgary);
		// date_yyc = mktimes("%m/%d/%Y", tzcalgary);
		t0 = gettemperature("/sys/devices/virtual/hwmon/hwmon0", "temp1_input");
		t1 = gettemperature("/sys/devices/virtual/hwmon/hwmon2", "temp1_input");
		t2 = gettemperature("/sys/devices/virtual/hwmon/hwmon4", "temp1_input");

		// status = smprintf("%s %s %s", bat, date_yyc, time_yyc);
		status = smprintf("%s %s", date_yyc, time_yyc);
		setstatus(status);

		free(t0);
		free(t1);
		free(t2);
		free(avgs);
		free(time_yyc);
		free(date_yyc);
		free(bat);
		free(tmar);
		free(tmutc);
		free(tmbln);
		free(status);
	}

	XCloseDisplay(dpy);

	return 0;
}

