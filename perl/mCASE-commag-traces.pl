#!/usr/bin/perl -w

use Time::Local;

$cfgFile = "configs/mCASE-commag-snapshot.cfg";
$totSimulation_ = 10;

sub executeSimulation
{
    my($numSim_) = @_;
	my($command,$path,$numSS,$auxSS,$auxSim);
	
    $command = "./bin/mcase ".$cfgFile;
#    printf("%s\n",$command);
    system($command);

	$path = "results/snapshots/";	
	for($numSS=1; $numSS <= 360; $numSS++) {
		$auxSS = sprintf("%04d",$numSS);
		$auxSim = sprintf("%03d",$numSim_);
		$command = "mv ".$path."snapshot_".$auxSS.".txt ".$path."UT_".$auxSim."_".$auxSS.".tr";
		#printf("%s\n",$command);
		system($command);
	}
}

# The main!!!
$t_ini = localtime;

for($nSim = 1 ; $nSim <= $totSimulation_ ; $nSim++) {
	printf("\n\nRunning mCASE for simulation %d\n",$nSim);
	&executeSimulation($nSim);
}



$t_fin = localtime;
print "\n","Tiempo inicio de la simulacion ",$t_ini,"\n";
print "Tiempo final de la simulacion ",$t_fin,"\n";
