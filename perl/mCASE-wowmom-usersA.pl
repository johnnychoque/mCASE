#!/usr/bin/perl -w

# El fichero mCASE.cfg debe tener configurado los siguientes valores para los pesos:
#
# [CONSTRAINTS]
# QUALITY=0.00
# HANDOVER=0.50
# OPERATOR=0.00
# LOAD=0.00
# PRICE=0.50

use Time::Local;

$cfgFile = "configs/mCASE-wowmom-usersA-ratio.cfg";
#$cfgFile = "configs/mCASE-wowmom-usersA-flat.cfg";
$totSimulation_ = 100;

sub changePriceConsumer
{
	my($prCon_) = @_;
    my($tempFILE, $command, $aux, $outputFile, $flag_);

    $tempFILE='temp.cfg';

    open(CFG_FILE, "$cfgFile") or die "Cannot open CFG file";
    open(OUT_FILE,">$tempFILE") or die "Cannot open Temp file";

    $flag_ = 0;
    while(<CFG_FILE>){
		if($_ =~ /\[*\]/) {
			if($_ =~ /USER0/) {
			$flag_ = 1; 
			} else {
			$flag_ = 0;
			}
		}
		if($_ =~ /PRICE=/ && $flag_ == 1) {
			$aux = sprintf("PRICE=%.2f\n",$prCon_);
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

sub changeHoDiscount
{
	my($hoDisc_) = @_;
    my($tempFILE, $command, $aux, $outputFile, $flag_);

    $tempFILE='temp.cfg';

    open(CFG_FILE, "$cfgFile") or die "Cannot open CFG file";
    open(OUT_FILE,">$tempFILE") or die "Cannot open Temp file";

    $flag_ = 0;
    while(<CFG_FILE>){
		if($_ =~ /\[*\]/) {
			if($_ =~ /HOPARAMS/) {
			$flag_ = 1; 
			} else {
			$flag_ = 0;
			}
		}
		if($_ =~ /DISCOUNT=/ && $flag_ == 1) {
			$aux = sprintf("DISCOUNT=%.2f\n",$hoDisc_);
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

sub changeSimulationNumbers
{
    my($numStrategy_, $numSimulation_) = @_;
    my($tempFILE, $command, $aux, $outputFile, $flag_);

    $tempFILE='temp.cfg';

    open(CFG_FILE, "$cfgFile") or die "Cannot open CFG file";
    open(OUT_FILE,">$tempFILE") or die "Cannot open Temp file";

    $flag_ = 0;
    while(<CFG_FILE>) {
		if($_ =~ /\[*\]/) {
			if($_ =~ /SIMULATION/) {
				$flag_ = 1; 
			} else {
				$flag_ = 0;
			}
		}

		if($_ =~ /STRATEGY=/ && $flag_ == 1) {
			$aux = sprintf("STRATEGY=%03d\n",$numStrategy_);
			print OUT_FILE $aux;
		} elsif($_ =~ /NUMSIM=/ && $flag_ == 1) {
			$aux = sprintf("NUMSIM=%03d\n",$numSimulation_);
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

sub runCommand
{
	my($command_) = @_;
	system($command_);
	
	if ( $? == -1 ) {
		print "\nCommand failed: $!\n";
		die "Please check the command ..."; 
	}
	elsif (($? >> 8) != 0) {
		print "\nSome parameter of the command is wrong\n";
		die "Please fix the wrong parameter";
	}

}

sub executeSimulation
{
    my($numStr_,$numUser_,$numSim_) = @_;
    my($command,$path,$numStr,$numUser,$numSim);
    
    $command = "./bin/mcase ".$cfgFile;
#    printf("%s\n",$command);
    system($command);

	$path = "results/stats/";
	$numStr = sprintf("%03d",$numStr_);
	$numUser = sprintf("%03d",$numUser_);
	$numSim = sprintf("%03d",$numSim_);
	
#	$command = "mv ".$path."serviceStats.out ".$path.$numStr."_serviceStats_".$numUser."_".$numSim.".out";
#	system($command);
	$command = "mv ".$path."userStats.out ".$path.$numStr."_userStats_".$numUser."_".$numSim.".out";
	&runCommand($command);
#	$command = "mv ".$path."linkQualityStats.out ".$path.$numStr."_linkQualityStats_".$numUser."_".$numSim.".out";
#	system($command);	
#	$command = "mv ".$path."hoPerServStats.out ".$path.$numStr."_hoPerServStats_".$numUser."_".$numSim.".out";
#	system($command);
	$command = "mv ".$path."hoPerUserStats.out ".$path.$numStr."_hoPerUserStats_".$numUser."_".$numSim.".out";
	&runCommand($command);
	$command = "mv ".$path."loadStats.out ".$path.$numStr."_loadStats_".$numUser."_".$numSim.".out";
	&runCommand($command);
#	$command = "mv ".$path."paramsList.out ".$path.$numStr."_paramsList_".$numUser."_".$numSim.".out";
#	system($command);
	$command = "mv ".$path."priceList.out ".$path.$numStr."_priceList_".$numUser."_".$numSim.".out";
	&runCommand($command);
#	$command = "mv ".$path."pricePerServStats.out ".$path.$numStr."_pricePerServStats_".$numUser."_".$numSim.".out";
#	system($command);
	$command = "mv ".$path."pricePerUserStats.out ".$path.$numStr."_pricePerUserStats_".$numUser."_".$numSim.".out";
	&runCommand($command);

}

# The main!!!
$t_ini = localtime;
# ratio
@prCon_  = (0.60, 0.80);
$totprCon_ = 2;
# flat
#@prCon_  = (0.60);
#$totprCon_ = 1;

&changeHoDiscount(0.5);
for ($i=0; $i<$totprCon_; $i++) {
	&changePriceConsumer($prCon_[$i]);
	for($nUsers = 1 ; $nUsers <= 10 ; $nUsers++) {
		&changeNumUsers($nUsers*25);
		for($numSim = 1 ; $numSim <= $totSimulation_ ; $numSim++) {
			printf("\n\nRunning mCASE for price consumer %.2f users %d simulation %d\n",$prCon_[$i],$nUsers*25,$numSim);
			&executeSimulation($prCon_[$i]*100,$nUsers*25,$numSim);
		}
	}
}

$t_fin = localtime;
print "\n","Tiempo inicio de la simulacion ",$t_ini,"\n";
print "Tiempo final de la simulacion ",$t_fin,"\n";
