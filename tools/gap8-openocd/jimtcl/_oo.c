/* autogenerated - do not edit */
#include <jim.h>
int Jim_ooInit(Jim_Interp *interp)
{
	if (Jim_PackageProvide(interp, "oo", "1.0", JIM_ERRMSG)) return JIM_ERR;
	return Jim_EvalSource(interp, "oo.tcl", 1, "\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"proc class {classname {baseclasses {}} classvars} {\n"
"	set baseclassvars {}\n"
"	foreach baseclass $baseclasses {\n"
"\n"
"		foreach method [$baseclass methods] { alias \"$classname $method\" \"$baseclass $method\" }\n"
"\n"
"		set baseclassvars [dict merge $baseclassvars [$baseclass classvars]]\n"
"\n"
"		proc \"$classname baseclass\" {} baseclass { return $baseclass }\n"
"	}\n"
"\n"
"\n"
"	set classvars [dict merge $baseclassvars $classvars]\n"
"	set vars [lsort [dict keys $classvars]]\n"
"\n"
"\n"
"\n"
"\n"
"	proc $classname {{cmd new} args} classname {\n"
"		if {![exists -command \"$classname $cmd\"]} {\n"
"			return -code error \"$classname, unknown command \\\"$cmd\\\": should be [join [$classname methods] \", \"]\"\n"
"		}\n"
"		tailcall \"$classname $cmd\" {*}$args\n"
"	}\n"
"\n"
"\n"
"	proc \"$classname new\" {{instvars {}}} {classname classvars} {\n"
"		set instvars [dict merge $classvars $instvars]\n"
"\n"
"\n"
"\n"
"\n"
"		set obj [ref $classname $classname \"$classname finalize\"]\n"
"		proc $obj {method args} {classname instvars} {\n"
"			if {![exists -command \"$classname $method\"]} {\n"
"				if {![exists -command \"$classname unknown\"]} {\n"
"					return -code error \"$classname, unknown method \\\"$method\\\": should be [join [$classname methods] \", \"]\"\n"
"				}\n"
"				return [\"$classname unknown\" $method {*}$args]\n"
"			}\n"
"			\"$classname $method\" {*}$args\n"
"		}\n"
"		if {[exists -command \"$classname constructor\"]} {\n"
"			$obj constructor\n"
"		}\n"
"		return $obj\n"
"	}\n"
"\n"
"	proc \"$classname finalize\" {ref classname} { $ref destroy }\n"
"\n"
"	proc \"$classname method\" {method arglist __body} classname {\n"
"		proc \"$classname $method\" $arglist {__body} {\n"
"\n"
"			if {![uplevel exists instvars]} {\n"
"				return -code error -level 2 \"\\\"[lindex [info level 0] 0]\\\" method called with no object\"\n"
"			}\n"
"			set self [lindex [info level -1] 0]\n"
"\n"
"\n"
"			foreach __ [$self vars] {upvar 1 instvars($__) $__}\n"
"			unset __\n"
"			eval $__body\n"
"		}\n"
"	}\n"
"\n"
"	proc \"$classname vars\" {} vars { return $vars }\n"
"	proc \"$classname classvars\" {} classvars { return $classvars }\n"
"	proc \"$classname classname\" {} classname { return $classname }\n"
"	proc \"$classname methods\" {} classname {\n"
"		lsort [lmap p [info procs \"$classname *\"] {\n"
"			lindex [split $p \" \"] 1\n"
"		}]\n"
"	}\n"
"\n"
"	$classname method destroy {} { rename $self \"\" }\n"
"	$classname method get {var} { set $var }\n"
"	$classname method eval {{locals {}} __code} {\n"
"		foreach var $locals { upvar 2 $var $var }\n"
"		eval $__code\n"
"	}\n"
"	return $classname\n"
"}\n"
"\n"
"\n"
"\n"
"proc super {method args} {\n"
"	upvar self self\n"
"	uplevel 2 [$self baseclass] $method {*}$args\n"
"}\n");
}
