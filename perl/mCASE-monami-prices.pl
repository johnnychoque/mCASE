#!/usr/bin/perl -w

use Time::Local;

$cfgFile = "configs/mCASE-monami-prices.cfg";
$totSimulation_ = 100;
$totStrategies_ = 4;
$PmaxGSM = 1;

@prefOP_  = (0.00, 0.00, 0.00, 0.00);
@handover_= (0.00, 0.00, 0.50, 0.50);
@quality_ = (0.00, 0.00, 0.00, 0.00);
@load_    = (0.00, 1.00, 0.00, 0.50);
@price_   = (1.00, 0.00, 0.50, 0.00);

sub changeAccessSelectionStrategy
{
    my($prefOP_,$handover_,$quality_,$load_,$price_) = @_;
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
			$aux = sprintf("OPERATOR=%.2f\n",$prefOP_);
			print OUT_FILE $aux;
		} elsif($_ =~ /HANDOVER=/ && $flag_ == 1) {
			$aux = sprintf("HANDOVER=%.2f\n",$handover_);
			print OUT_FILE $aux;
		} elsif($_ =~ /QUALITY=/ && $flag_ == 1) {
			$aux = sprintf("QUALITY=%.2f\n",$quality_);
			print OUT_FILE $aux;
		} elsif($_ =~ /LOAD=/ && $flag_ == 1) {
			$aux = sprintf("LOAD=%.2f\n",$load_);
			print OUT_FILE $aux;
		} elsif($_ =~ /PRICE=/ && $flag_ == 1) {
			$aux = sprintf("PRICE=%.2f\n",$price_);
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

sub changePriceRates
{
	my($priceGSM_,$priceWIFI_) = @_;
    my($tempFILE, $command, $aux, $outputFile, $flag_);

    $tempFILE='temp.cfg';

    open(CFG_FILE, "$cfgFile") or die "Cannot open CFG file";
    open(OUT_FILE,">$tempFILE") or die "Cannot open Temp file";

    $flag_ = 0;
    while(<CFG_FILE>){
		if($_ =~ /\[*\]/) {
			if($_ =~ /OPERATOR0/) {
			$flag_ = 1; 
			} else {
			$flag_ = 0;
			}
		}
		if($_ =~ /PRICEBS0=/ && $flag_ == 1) {
			$aux = sprintf("PRICEBS0=%.2f\n",$priceWIFI_);
			print OUT_FILE $aux;
		} elsif($_ =~ /PRICEBS1=/ && $flag_ == 1) {
			$aux = sprintf("PRICEBS1=%.2f\n",$priceGSM_);
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
    my($numStrategy_, $priceRate_, $numSimulation_) = @_;
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
		} elsif($_ =~ /PRICERATE=/ && $flag_ == 1) {
			$aux = sprintf("PRICERATE=%03d\n",$priceRate_);
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
	
    my($numStr_,$perRate_,$numSim_) = @_;
    my($command,$path,$numStr,$perRate,$numSim);
    
    $command = "./bin/mcase ".$cfgFile;
#    printf("%s\n",$command);
    system($command);

	$path = "results/stats/";
	$numStr = sprintf("%03d",$numStr_);
	$perRate = sprintf("%03d",$perRate_);
	$numSim = sprintf("%03d",$numSim_);
	
	$command = "mv ".$path."serviceStats.out ".$path.$numStr."_serviceStats_".$perRate."_".$numSim.".out";
	system($command);
#	$command = "mv ".$path."userStats.out ".$path.$numStr."_userStats_".$perRate."_".$numSim.".out";
#	system($command);
#	$command = "mv ".$path."linkQualityStats.out ".$path.$numStr."_linkQualityStats_".$perRate."_".$numSim.".out";
#	system($command);	
	$command = "mv ".$path."hoPerServStats.out ".$path.$numStr."_hoPerServStats_".$perRate."_".$numSim.".out";
	system($command);
#	$command = "mv ".$path."hoPerUserStats.out ".$path.$numStr."_hoPerUserStats_".$perRate."_".$numSim.".out";
#	system($command);
	$command = "mv ".$path."loadStats.out ".$path.$numStr."_loadStats_".$perRate."_".$numSim.".out";
	system($command);	
	$command = "mv ".$path."paramsList.out ".$path.$numStr."_paramsList_".$perRate."_".$numSim.".out";
	system($command);
	$command = "mv ".$path."priceList.out ".$path.$numStr."_priceList_".$perRate."_".$numSim.".out";
	system($command);
	$command = "mv ".$path."pricePerServStats.out ".$path.$numStr."_pricePerServStats_".$perRate."_".$numSim.".out";
	system($command);
#	$command = "mv ".$path."pricePerUserStats.out ".$path.$numStr."_pricePerUserStats_".$perRate."_".$numSim.".out";
#	system($command);	
}

# The main!!!
$t_ini = localtime;
for($nStr = 1 ; $nStr <= $totStrategies_ ; $nStr++) {
	&changeAccessSelectionStrategy($prefOP_[$nStr-1], $handover_[$nStr-1], $quality_[$nStr-1], $load_[$nStr-1], $price_[$nStr-1]);
	for($pRate = 1 ; $pRate <= 10 ; $pRate++) {
		$PmaxWIFI = $PmaxGSM*$pRate*0.1;
		&changePriceRates($PmaxGSM,$PmaxWIFI);
		for($nSim = 1 ; $nSim <= $totSimulation_ ; $nSim++) {
			printf("\n\nRunning mCASE for strategy %d price %.2f simulation %d\n",$nStr,$pRate*0.1,$nSim);
			#&changeSimulationNumbers($nStr,$pRate,$nSim);
			&executeSimulation($nStr,$pRate,$nSim);
		}
	}
}

$t_fin = localtime;
print "\n","Tiempo inicio de la simulacion ",$t_ini,"\n";
print "Tiempo final de la simulacion ",$t_fin,"\n";
