#!/usr/bin/perl -w

use Time::Local;

$cfgFile = "configs/mCASE-WCNC13-snapshot.cfg";
#$totSimulation_ = 10;
$totSimulation_ = 100;

sub changeNumUsers
{
    my($numUsers_) = @_;
    my($tempFILE, $command, $aux, $outputFile, $flag_);

    $tempFILE='temp.cfg';

    open(CFG_FILE, "$cfgFile") or die "Cannot open CFG file";
    open(OUT_FILE,">$tempFILE") or die "Cannot open Temp file";

    $flag_ = 0;
    while(<CFG_FILE>){
		if($_ =~ /\[*\]/) {
			if($_ =~ /USERS/) {
			$flag_ = 1; 
			} else {
			$flag_ = 0;
			}
		}
		if($_ =~ /NUMBER=/ && $flag_ == 1) {
			$aux = sprintf("NUMBER=%d\n",$numUsers_);
			print OUT_FILE $aux;
		} else {
			print OUT_FILE $_;
		}
	}
	    
    close(CFG_FILE);
    close(OUT_FILE);
    $command = "rm -f ".$cfgFile;
    system($command);
    $command = "mv ".$tempFILE." ".$cfgFile;
    system($command);

}

sub executeSimulation
{
    my($numUser_,$numSim_) = @_;
	
    $command = "./bin/mcase ".$cfgFile;
#    printf("%s\n",$command);
    system($command);

	$path = "results/snapshots/";	
	for($numSS=1; $numSS <= 360; $numSS++) {
		$auxSS = sprintf("%04d",$numSS);
		$auxUser = sprintf("%03d",$numUser_);
		$auxSim = sprintf("%03d",$numSim_);
		$command = "mv ".$path."snapshot_".$auxSS.".txt ".$path."UT_".$auxUser."_".$auxSim."_".$auxSS.".tr";
		#printf("%s\n",$command);
		system($command);
	}
}


# The main!!!
$t_ini = localtime;

#for($nUsers = 1 ; $nUsers <= 10 ; $nUsers++) {
for($nUsers = 15 ; $nUsers <= 15 ; $nUsers++) {
	#$numUsers = $nUsers*20;
	$numUsers = $nUsers*10;
	&changeNumUsers($numUsers);
	for($numSim = 1 ; $numSim <= $totSimulation_ ; $numSim++) {
		printf("\n\nRunning mCASE for users %d simulation %d\n",$numUsers,$numSim);	
		&executeSimulation($numUsers,$numSim);
	}
}


$t_fin = localtime;
print "\n","Tiempo inicio de la simulacion ",$t_ini,"\n";
print "Tiempo final de la simulacion ",$t_fin,"\n";
