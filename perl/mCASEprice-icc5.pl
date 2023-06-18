#!/usr/bin/perl -w

use Time::Local;

$cfgFile = "configs/mCASE-precios-icc5.cfg";
$totSimulation_ = 100;
$totStrategies_ = 4;
$PmaxGSM = 1;

@prefOP_  = (0.00, 0.00, 0.00, 0.00);
@handover_= (0.00, 0.00, 0.20, 0.20);
@quality_ = (0.00, 0.00, 0.00, 0.00);
@load_    = (0.00, 1.00, 0.00, 0.80);
@price_   = (1.00, 0.00, 0.80, 0.00);

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
#    my($currenti_) = @_;
    my($command);
    
    $command = "./bin/mcase ".$cfgFile;
#    printf("%s\n",$command);
    system($command);
}

# The main!!!
$t_ini = localtime;
for($numStr = 1 ; $numStr <= $totStrategies_ ; $numStr++) {
	&changeAccessSelectionStrategy($prefOP_[$numStr-1], $handover_[$numStr-1], $quality_[$numStr-1], $load_[$numStr-1], $price_[$numStr-1]);
	for($pRate = 1 ; $pRate <= 10 ; $pRate++) {
		$PmaxWIFI = $PmaxGSM*$pRate*0.1;
		&changePriceRates($PmaxGSM,$PmaxWIFI);
		for($numSim = 1 ; $numSim <= $totSimulation_ ; $numSim++) {
			printf("\n\nRunning mCASE for strategy %d price %.2f simulation %d\n",$numStr,$pRate*0.1,$numSim);
			&changeSimulationNumbers($numStr,$pRate,$numSim);
			&executeSimulation();
		}
	}
}

$t_fin = localtime;
print "\n","Tiempo inicio de la simulacion ",$t_ini,"\n";
print "Tiempo final de la simulacion ",$t_fin,"\n";
