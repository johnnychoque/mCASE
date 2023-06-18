#!/usr/bin/perl -w

$cfgFile = "prueba2.cfg";


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


&changePercentConsumer(0.2);
&changePercentBusiness(0.8);