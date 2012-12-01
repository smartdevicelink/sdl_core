#!/usr/bin/perl

# modified Wed Oct 24 10:49:58 EEST 2012

use FileHandle;
use File::Slurp;
use File::Temp;
use XML::Simple;
use Sort::Topological qw(toposort);
use JSON::XS;
use Data::Dumper;
use File::Spec;
use Cwd;
use Data::Compare;

use strict;


my %config;

# args parse
sub args_parse
{
   my @args=@ARGV;
   my $t;
   while($t=shift @args)
   {
     if($t eq '-1')    {  $config{'xml1'}=shift @args;   next;   }
     if($t eq '-2')    {  $config{'xml2'}=shift @args;   next;   }
#     if($t eq '-n1')    {  $config{'ns1'}=shift @args;   next;   }
#     if($t eq '-n2')    {  $config{'ns2'}=shift @args;   next;   }
#     if($t eq '-p')    {  $config{'path'}=shift @args; $config{'pimpl'}=shift @args;   next;   }
     die 'unknown in command line '.$t;
   }
}

args_parse();

die 'no input' if not exists $config{'xml1'};
die 'no input' if not exists $config{'xml2'};

$config{'path'}='include' if not exists $config{'path'};
$config{'pimpl'}='src' if not exists $config{'pimpl'};

$config{'ns1'}='NsAppLinkRPCV1' if not exists $config{'ns1'};
$config{'ns2'}='NsAppLinkRPCV2' if not exists $config{'ns2'};

my %itypes= map { $_ => 1} qw(Boolean Integer String Float);

sub load
{
  my $xn=shift;

  my $xml=XMLin($xn,ContentKey => '__content', KeyAttr => [], ForceArray =>1, KeepRoot => 1);
  die sprintf 'cant parse xml %s',$xn if not ref $xml;
  die sprintf "interface section not found in xml %s",$xn if not exists $xml->{'interface'};

  $xml=$xml->{'interface'}->[0];

  my @table;
  my %types;
  my %exts;
  my %dmt;
  my %rdmt;
  my %dmm;

  foreach(qw(name date version))
  {
    die "$_ attribute found in tags" if ref $xml->{$_};
    $exts{$_}=$xml->{$_} or die "$_ attribute not specified";
  }

  push @table,[$_->{'name'},$_->{'messagetype'},$_->{'param'},$_->{'description'}] foreach @{$xml->{'function'}};

  for(my $i=0;$i<@table;$i++)
  {
    my $k=@table[$i];
    my $n=$k->[0];
    my $s=sprintf "%s : %s",$n,$k->[1];
    $dmm{$s}={};
    foreach(@{$k->[2]})
    {
      $dmm{$s}->{$_->{'type'}}=1 if not exists $itypes{$_->{'type'}};
    }
  }

#die Dumper $xml->{'enum'};

  foreach(@{$xml->{'enum'}})
  {
    my $k=$_;
    die sprintf "duplicate definition %s",$k->{'name'} if exists $types{$k->{'name'}};
    $types{$k->{'name'}}->{'list'}=[];
    $types{$k->{'name'}}->{'ldesc'}=[];
    $types{$k->{'name'}}->{'iname'}=[];

    $types{$k->{'name'}}->{'type'}='enum';
    push @{$types{$k->{'name'}}->{'list'}}, map { $_->{'name'} } @{$k->{'element'}};
    push @{$types{$k->{'name'}}->{'ldesc'}}, map { $_->{'description'} } @{$k->{'element'}};
    push @{$types{$k->{'name'}}->{'iname'}}, map { $_->{'internal_name'} } @{$k->{'element'}};
  
    for(my $i=0;$i<@{$types{$k->{'name'}}->{'list'}};$i++)
    {
      $types{$k->{'name'}}->{'iname'}->[$i]=$types{$k->{'name'}}->{'list'}->[$i] if not defined $types{$k->{'name'}}->{'iname'}->[$i];
      if($types{$k->{'name'}}->{'iname'}->[$i]!~/^[a-zA-Z]/)
      {
        printf STDERR "ill-defined enum value %s in enum %s, fixing...\n",$types{$k->{'name'}}->{'iname'}->[$i],$k->{'name'};
        $types{$k->{'name'}}->{'iname'}->[$i]="FIX_".$types{$k->{'name'}}->{'iname'}->[$i];
      }
    }

    $types{$k->{'name'}}->{'descr'}=$k->{'description'} if exists $k->{'description'};
  }

  foreach(@{$xml->{'struct'}})
  {
    my $k=$_;
    die sprintf "duplicate definition %s",$k->{'name'} if exists $types{$k->{'name'}};  
    $types{$k->{'name'}}->{'list'}={};
    $types{$k->{'name'}}->{'type'}='struct';
    %{$types{$k->{'name'}}->{'list'}}= map { $_->{'name'} , $_ } @{$k->{'param'}};
    $types{$k->{'name'}}->{'descr'}=$k->{'description'} if exists $k->{'description'};
  }

  foreach(@{$xml->{'variant'}})
  {
    my $k=$_;
    die sprintf "duplicate definition %s",$k->{'name'} if exists $types{$k->{'name'}};
    $types{$k->{'name'}}->{'list'}={};
    $types{$k->{'name'}}->{'type'}='variant';
#  die Dumper $k->{'element'};
    %{$types{$k->{'name'}}->{'list'}}= map { $_->{'name'} , $_ } @{$k->{'param'}};
    $types{$k->{'name'}}->{'descr'}=$k->{'description'} if exists $k->{'description'};
  
    $types{$k->{'name'}}->{'list'}->{$_}->{'ename'}= $types{$k->{'name'}}->{'list'}->{$_}->{'name'}
      foreach keys %{$types{$k->{'name'}}->{'list'}};

    $types{$k->{'name'}}->{'ldesc'}=[];
    push @{$types{$k->{'name'}}->{'ldesc'}}, map { $_->{'description'} } @{$k->{'param'}};
  }

#fixing conflicts

  {
    my %tmp;
    foreach(sort keys %types)
    {
      my $n=$_;
      if($types{$n}->{'type'} eq 'enum')
      {
        $tmp{$_}=['enum',$n,$_] foreach @{$types{$n}->{'iname'}};
        next;
      }

      if($types{$n}->{'type'} eq 'variant')
      {
#      $tmp{$_}++ foreach @{$types{$n}->{'iname'}};
        $tmp{$types{$n}->{'list'}->{$_}->{'ename'}}=['variant',$n,$_] foreach keys %{$types{$n}->{'list'}};
        next;
      }
    }
#    die Dumper \%tmp;

    foreach(sort keys %types)
    {
      my $n=$_;
      next if not exists $tmp{$n};
      printf STDERR "naming conflict for %s class/enumerator: %s, not fixable\n",$n,Dumper $tmp{$n};
      die Dumper $types{$tmp{$n}->[1]}->{'list'};
    }
  } 

  undef $xml;

  foreach(sort keys %types)
  {
    $dmt{$_}={};
    next if $types{$_}->{'type'} eq 'enum';
    my $n=$_;

    foreach(values %{$types{$n}->{'list'}})
    {
      next if exists $itypes{$_->{'type'}};
      my $k=$_;
      die sprintf "unknown type in structure definition %s:%s %s",$n,$k->{'name'},$k->{'type'} if not exists $types{$k->{'type'}};
      $dmt{$n}->{$k->{'type'}}=1;
      $rdmt{$k->{'type'}}=[] if not exists $rdmt{$k->{'type'}};
      push @{$rdmt{$k->{'type'}}},$n;
    }
  }

  foreach(keys %dmm)
  {
    my $k=$_;
    my $v=$dmm{$_};
    foreach(keys %$v)
    {
      die sprintf "type '%s' not found in method %s",$_,$k if not exists $types{$_};
    }
  }

#die Dumper %types;

  my $rv=[{},{},[],{},{},{}];
  %{$rv->[0]}=%exts;
  %{$rv->[1]}=%types;
  @{$rv->[2]}=@table;
  %{$rv->[3]}=%dmt;
  %{$rv->[4]}=%dmm;
  %{$rv->[5]}=%rdmt;

  return $rv;
}


my $t1=load($config{'xml1'});
my $t2=load($config{'xml2'});


my @table=@{$t1->[2]};
my %types=%{$t1->[1]};
my %exts=%{$t1->[0]};
my %dmt=%{$t1->[3]};
my %dmm=%{$t1->[4]};

#die Dumper \%dmt;

foreach (sort keys %{$t2->[0]})
{
  if(not exists $exts{$_})
  {
    $exts{$_}=$t2->[0]->{$_};
    next;
  }
  next if $exts{$_} eq $t2->[0]->{$_};
  $exts{$_}.=' / '.$t2->[0]->{$_};
}


my %mod;

my @l=sort keys %{$t2->[1]};

my @order=qw(a b c d e f);

my %horder=map { my $t=[]; push @$t,keys %{$t2->[3]{$_}}; $_ => $t } sort keys %{$t2->[3]};

sub topo_children { @{$horder{$_[0]} || []}; }
my @ts=reverse toposort(\&topo_children,\@l);

#die Dumper(\@ts);

sub genfix
{
  my $x=shift;
  $x.='_fix';
  $x.='_fix' while exists $types{$x};
  return $x;
}


foreach (@ts)
{
  my $k=$_;
  my $s=$k;
  $k=$mod{$k} while exists $mod{$k};
  my $v=$t2->[1]->{$s};

  if(exists $mod{$s})
  {
     foreach(@{$t2->[5]->{$s}})
     {
       my $z=$_;
       next if exists $mod{$z};
       $mod{$z}=genfix($z);
     }
  }

  if($v->{'type'} ne 'enum')
  {
    foreach(keys %{$v->{'list'}})
    {
      my $n=$_;
      my $vn=$v->{'list'}->{$n};
      next if exists $itypes{$v->{'list'}->{$n}->{'type'}};

      my $z=$v->{'list'}->{$n}->{'type'};
      $z=$mod{$z} 
        while exists $mod{$z};# and $mod{$z}->[0]!=1;
      $v->{'list'}->{$n}->{'type'}=$z;
    }
  }

  if(not exists $types{$k})
  {
    $types{$k}=$v;
#    delete $t2->[1]->{$k}; #='moved';
#todo: add namespace

    next;
  }

  my $v0=$types{$k};
  if($v0->{'type'} ne $v->{'type'})
  {
fix:
    printf STDERR "fixing type conflict for %s -> %s_fix\n",$k,$k;
    my $nk=genfix($k);

    $mod{$s}=$nk;
    $types{$nk}=$v;
    
    next if not exists $t2->[5]->{$s};

    my @l=@{$t2->[5]->{$s}};

    foreach(@{$t2->[5]->{$s}})
    {
       my $z=$_;
       next if exists $mod{$z};
       $mod{$z}=genfix($z);
    }

#    $mov{$nk}=$nk;
    next;
  }

# struct -> list ->
# variant -> list ->

  if($v->{'type'} eq 'enum')
  {
    goto fix if @{$v->{'list'}}!=@{$v0->{'list'}} or @{$v->{'iname'}}!=@{$v0->{'iname'}};
    for(my $t=0;$t<@{$v->{'list'}};$t++)
    {
      goto fix if $v->{'list'}->[$t] ne $v0->{'list'}->[$t];
    }

    for(my $t=0;$t<@{$v->{'iname'}};$t++)
    {
      goto fix if $v->{'iname'}->[$t] ne $v0->{'iname'}->[$t];
    }
    next;
  }

#  printf "0 %s\n",scalar keys %{$v->{'list'}};
#  print Dumper($v->{'list'});
  foreach(keys %{$v->{'list'}})
  {
    my $n=$_;
    my $vn=$v->{'list'}->{$n};
    next if exists $itypes{$v->{'list'}->{$n}->{'type'}};

    my $z=$v->{'list'}->{$n}->{'type'};
    $z=$mod{$z} 
      while exists $mod{$z};# and $mod{$z}->[0]!=1;
    $v->{'list'}->{$n}->{'type'}=$z;
  }

#    if $s eq 'VehicleType';

  goto fix if Compare($v->{'list'},$v0->{'list'},{ ignore_hash_keys => ['description','platform','element','issue'] });
  printf STDERR "duplicate structures %s, no fixing required, simple skipped\n",$k;
  
}

#die Dumper \%types;

#foreach(keys %mod)


my %q0=map { sprintf("%s : %s",$_->[0],$_->[1]) => $_ } @{$t1->[2]};
my %q=map { sprintf("%s : %s",$_->[0],$_->[1]) => $_ } @{$t2->[2]};

my %fixed;

foreach(sort keys %q)
{
  my $k=$_;
  my $v=$q{$k};
  for(my $i=0;$i<@{$v->[2]};$i++)
  {
    my $t=$v->[2]->[$i]->{'type'};
    next if exists $itypes{$t};
    next if not exists $mod{$t};
    $t=$mod{$t} while exists $mod{$t};
    $v->[2]->[$i]->{'type'}=$t;
  }
}

foreach(sort %q)
{
  my $k=$_;
  my $v=$q{$k};

  my $nk=$v->[0];
  $nk=$k.'_fix';

  next if not exists $q0{$k};
  my $v0=$q0{$k};
  
  if($v->[0] ne $v0->[0]) #or $v->[1] ne $v0->[1])
  {
fix:
    printf STDERR "fixing method conflict for %s -> %s\n",$k,$nk;
    $fixed{$v->[0]}=$nk;
    next;
  }

  goto fix if not Compare($v->[2],$v0->[2],{ ignore_hash_keys => ['description','platform','element'] });
  printf STDERR "duplicate method %s\n",$k;
}

#die Dumper \@table;

foreach(sort keys %q)
{
  my $k=$_;
  my $v=$q{$k};
 
  $v->[0].='_fix' if exists $fixed{$v->[0]};
  push @table,$v;
}

my %res;
$res{'interface'}=[{}];
my $r=$res{'interface'}->[0];

%$r=%exts;

$r->{'function'}=[];

foreach(@table)
{
  my $k=$_;
  my $x={};
  $x->{'name'}=$k->[0];
  $x->{'messagetype'}=$k->[1];
  $x->{'param'}=$k->[2];
  $x->{'description'}=$k->[3];
  push @{$r->{'function'}},$x;
}

$r->{'enum'}=[];
$r->{'variant'}=[];
$r->{'struct'}=[];

foreach(sort keys %types)
{
  my $k=$_;
  my $v=$types{$k};

  my $x={};

  $x->{'name'}=$k;
  $x->{'description'}=[];
  $x->{'description'}=$v->{'descr'} if exists $v->{'descr'};

  if($v->{'type'} eq 'enum')
  {
    $x->{'element'}=[];
    for(my $i=0;$i<@{$v->{'list'}};$i++)
    {
      my $z={};
      $z->{'name'}=$v->{'list'}->[$i];
      $z->{'internal_name'}=$v->{'iname'}->[$i];
      $z->{'description'}=$v->{'ldesc'}->[$i] if defined $v->{'ldesc'}->[$i];
      push @{$x->{'element'}},$z;
    }
  } 
  elsif($v->{'type'} eq 'struct')
  {
    $x->{'param'}=[];
    push @{$x->{'param'}},$v->{'list'}->{$_} foreach sort keys %{$v->{'list'}};
  }
  else					# variant, not deep tested yet
  {
    $x->{'param'}=[];
    push @{$x->{'param'}},$v->{'list'}->{$_} foreach sort keys %{$v->{'list'}};
  }

  push @{$r->{$v->{'type'}}},$x;
}


#die Dumper \@table;
print XMLout(\%res,'KeepRoot' => 1);

print STDERR "done\n";
#die Dumper \%types;
exit 0;


