#!/usr/bin/perl -w

use Time::Local;

$cfgFile = "configs/mCASE-pimrc-usersB.cfg";
$totSimulation_ = 100;
#$totStrategies_ = 2;

#@prefOP_  = (0.00, 0.00, 0.00, 0.00);
#@handover_= (0.00, 0.00, 0.20, 0.20);
#@quality_ = (0.00, 0.00, 0.00, 0.00);
#@load_    = (0.00, 1.00, 0.00, 0.80);
#@price_   = (1.00, 0.00, 0.80, 0.00);

#@handover_= (0.20, 0.50, 0.80);
#@price_   = (0.80, 0.50, 0.20);

sub changeAccessSelectionStrategy
{
#    my($handover_,$price_) = @_;
    my($tempFILE, $command, $aux, $outputFile, $flag_);

    $tempFILE='temp.cfg';

    open(CFG_FILE, "$cfgFile") or die "Cannot open CFG file";
    open(OUT_FILE,">$tempFILE") or die "Cannot open Temp file";

    $flag_ = 0;
    while(<CFG_FILE>){
		if($_ =~ /\[*\]/) {
			if($_ =~ /CONSTRAINTS/) {
			$flag_ = 1; 
			} else {
			$flag_ = 0;
			}
		}
		if($_ =~ /OPERATOR=/ && $flag_ == 1) {
			$aux = sprintf("OPERATOR=%.2f\n",0);
			print OUT_FILE $aux;
		} elsif($_ =~ /HANDOVER=/ && $flag_ == 1) {
			$aux = sprintf("HANDOVER=%.2f\n",0.2);
			print OUT_FILE $aux;
		} elsif($_ =~ /QUALITY=/ && $flag_ == 1) {
			$aux = sprintf("QUALITY=%.2f\n",0);
			print OUT_FILE $aux;
		} elsif($_ =~ /LOAD=/ && $flag_ == 1) {
			$aux = sprintf("LOAD=%.2f\n",0);
			print OUT_FILE $aux;
		} elsif($_ =~ /PRICE=/ && $flag_ == 1) {
			$aux = sprintf("PRICE=%.2f\n",0.8);
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

sub changePercentConsumer
{
	my($perCon_) = @_;
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
		if($_ =~ /PROBABILITY=/ && $flag_ == 1) {
			$aux = sprintf("PROBABILITY=%.2f\n",$perCon_);
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

sub changePercentBusiness
{
	my($perBus_) = @_;
    my($tempFILE, $command, $aux, $outputFile, $flag_);

    $tempFILE='temp.cfg';

    open(CFG_FILE, "$cfgFile") or die "Cannot open CFG file";
    open(OUT_FILE,">$tempFILE") or die "Cannot open Temp file";

    $flag_ = 0;
    while(<CFG_FILE>){
		if($_ =~ /\[*\]/) {
			if($_ =~ /USER1/) {
			$flag_ = 1; 
			} else {
			$flag_ = 0;
			}
		}
		if($_ =~ /PROBABILITY=/ && $flag_ == 1) {
			$aux = sprintf("PROBABILITY=%.2f\n",$perBus_);
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

sub executeSimulation
{
    my($numStr_,$pBuss_,$numSim_) = @_;
    my($command,$path,$numStr,$pBuss,$numSim);
    
    $command = "./bin/mcase ".$cfgFile;
#    printf("%s\n",$command);
    system($command);

	$path = "results/stats/";
	$numStr = sprintf("%03d",$numStr_);
	$pBuss = sprintf("%03d",$pBuss_);
	$numSim = sprintf("%03d",$numSim_);
	
	$command = "mv ".$path."serviceStats.out ".$path.$numStr."_serviceStats_".$pBuss."_".$numSim.".out";
	system($command);
	$command = "mv ".$path."userStats.out ".$path.$numStr."_userStats_".$pBuss."_".$numSim.".out";
	system($command);
	$command = "mv ".$path."linkQualityStats.out ".$path.$numStr."_linkQualityStats_".$pBuss."_".$numSim.".out";
	system($command);	
	$command = "mv ".$path."handoverStats.out ".$path.$numStr."_handoverStats_".$pBuss."_".$numSim.".out";
	system($command);
	$command = "mv ".$path."loadStats.out ".$path.$numStr."_loadStats_".$pBuss."_".$numSim.".out";
	system($command);	
	$command = "mv ".$path."paramsList.out ".$path.$numStr."_paramsList_".$pBuss."_".$numSim.".out";
	system($command);
	$command = "mv ".$path."priceList.out ".$path.$numStr."_priceList_".$pBuss."_".$numSim.".out";
	system($command);
	$command = "mv ".$path."pricePerServStats.out ".$path.$numStr."_pricePerServStats_".$pBuss."_".$numSim.".out";
	system($command);
	$command = "mv ".$path."pricePerUserStats.out ".$path.$numStr."_pricePerUserStats_".$pBuss."_".$numSim.".out";
	system($command);

}

# The main!!!
$t_ini = localtime;
&changeAccessSelectionStrategy();
#for($numStr = 1 ; $numStr <= $totStrategies_ ; $numStr++) {
	#&changeAccessSelectionStrategy($handover_[$numStr-1], $price_[$numStr-1]);
for($prCon = 6 ; $prCon <= 8 ; $prCon++) {
	&changePriceConsumer($prCon*0.1);
	for($nPerc = 2 ; $nPerc <= 8 ; $nPerc++) {
		&changePercentConsumer(1.0-$nPerc*0.1);
		&changePercentBusiness($nPerc*0.1);
		for($nSim = 1 ; $nSim <= $totSimulation_ ; $nSim++) {
			printf("\n\nRunning mCASE for price consumer %.2f bussines %.2f simulation %d\n",$prCon*0.1,$nPerc*0.1,$nSim);
			#&changeSimulationNumbers($prCon,$nSim);
			&executeSimulation($prCon*10,$nPerc*10,$nSim);
		}
	}
}

$t_fin = localtime;
print "\n","Tiempo inicio de la simulacion ",$t_ini,"\n";
print "Tiempo final de la simulacion ",$t_fin,"\n";
