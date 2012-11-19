#!/usr/bin/perl

# modified Wed Oct 24 10:49:58 EEST 2012

use FileHandle;
use File::Slurp;
use File::Temp;
use XML::Simple;
#use Sort::Topological qw(toposort);
use JSON::XS;
use Data::Dumper;
use File::Spec;
use Cwd;

use strict;

#$SIG{PIPE} = 'IGNORE';


my %config;

# args parse
sub args_parse
{
   my @args=@ARGV;
   my $t;
   while($t=shift @args)
   {
     if($t eq '-i')    {  $config{'xml'}=shift @args;   next;   }
     if($t eq '-r')   {  $config{'random'}=shift @args; next; }
     if($t eq '-c')   {  $config{'loops'}=shift @args; next; }
     if($t eq '-log')   {  $config{'log'}=shift @args; next; }
     if($t eq '-lib')   {  $config{'lib'}=shift @args; next; }
     if($t eq '-n')   {  $config{'ns'}=shift @args; next; }
     if($t eq '-s')   {  $config{'style'}=shift @args; next; }
     if($t eq '-sh')   {  $config{'sh'}=shift @args; next; }
     if($t eq '-sc')   {  $config{'sc'}=shift @args; next; }
     if($t eq '-ee')   {  $config{'ee'}=1; next; }
     if($t eq '-p')    {  $config{'pver'}=shift @args;   next;   }
     if($t eq '-g')    {  $config{'pragma'}=1;   next;   }
     if($t eq '-a')    {  $config{'abs'}=1;   next;   }

     if($t eq '-md')   {  $config{'dryrun'}=shift @args; $config{'mode'}='dryrun'; next; }
     if($t eq '-mf')   {  $config{'fuzzer'}=shift @args; $config{'mode'}='fuzzer'; next; }
     if($t eq '-mg')   {  $config{'generate'}=shift @args; $config{'pimpl'}=shift @args; $config{'mode'}='generate'; next; }

     die 'unknown in command line '.$t;
   }
}

args_parse();

die 'no input' if not exists $config{'xml'};
die 'nothing to do' if not exists $config{'mode'};
die sprintf "cant open input xml %s",$config{'xml'} if not -e $config{'xml'};

# parse xml
my $xml=XMLin($config{'xml'},ContentKey => '__content', KeyAttr => [], ForceArray =>1, KeepRoot => 1);
die 'cant parse xml' if not ref $xml;

die "interface section not found in xml" if not exists $xml->{'interface'};

$xml=$xml->{'interface'}->[0];

my @table;
my %types;
my %exts;


foreach(qw(name date version))
{
  die "$_ attribute found in tags" if ref $xml->{$_};
  $exts{$_}=$xml->{$_} or die "$_ attribute not specified";
}

my %json=(
  'Float' => 'Double',
  'Integer' => 'Int',
  'Boolean' => 'Bool',
  'String' => 'String'
);

my %json2=(
  'Float' => 'Numeric',
  'Integer' => 'Int',
  'Boolean' => 'Bool',
  'String' => 'String'
);


push @table,[$_->{'name'},$_->{'messagetype'},$_->{'param'},$_->{'description'}] foreach @{$xml->{'function'}};


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


my $rnd= exists $config{'random'} ? $config{'random'} : $$ ^ time;


goto generate if $config{'mode'} eq 'generate';


printf STDERR "random is %u\n",$rnd;
srand($rnd);


my $maxstring=10;
my $maxarray=10;

sub generate_type_valid
{
  my $p=shift;
  return undef if($p->{'mandatory'} eq 'false' and (int rand 2)==0);
  
  if($p->{'array'} eq 'true')
  {
    my $minsize=0;
    my $maxsize=$maxarray;
    
    $minsize=$p->{'minsize'} if exists $p->{'minsize'};
    $maxsize=$p->{'maxsize'} if exists $p->{'maxsize'};
    my $size= $minsize+ int rand ($maxsize-$minsize);
    my $rv=[];

    my $tp= {%$p};
    $tp->{'array'}='false';
    $tp->{'mandatory'}='true';
    
    push @$rv,generate_type_valid($tp) while $size--;
    return $rv;
  }
  
  return (JSON::XS::false,JSON::XS::true)[int rand 2] if $p->{'type'} eq 'Boolean';
  if($p->{'type'} eq 'Float')
  {
    my $minsize=0;
    my $maxsize=100;
    
    $minsize=$p->{'minvalue'} if exists $p->{'minvalue'};
    $maxsize=$p->{'maxvalue'} if exists $p->{'maxvalue'};
    return ($minsize+ rand ($maxsize-$minsize));
  }

  if($p->{'type'} eq 'Integer')
  {
    my $minsize=0;
    my $maxsize=100;
    
    $minsize=$p->{'minvalue'} if exists $p->{'minvalue'};
    $maxsize=$p->{'maxvalue'} if exists $p->{'maxvalue'};

    return ($minsize+ int rand ($maxsize-$minsize));
  }

  if($p->{'type'} eq 'String')
  {
    my $maxl=$maxstring;
    $maxl= $p->{'maxlength'} if exists $p->{'maxlength'};
    return chr(1+int rand 127) x int rand $maxl;
#    return chr(int rand 256) x int rand $maxl;
  }

  die "not defined type: ".Dumper $p if not exists $types{$p->{'type'}};
  
  if($types{$p->{'type'}}->{'type'} eq 'enum')
  {
     return $types{$p->{'type'}}->{'list'}->[int rand @{$types{$p->{'type'}}->{'list'}}]
  }

  if($types{$p->{'type'}}->{'type'} eq 'variant')
  {
     my @z=sort keys %{$types{$p->{'type'}}->{'list'}};
     my $q=@z[int rand @z];
     return { $q => generate_type_valid($types{$p->{'type'}}->{'list'}->{$q}) };
  }


  if($types{$p->{'type'}}->{'type'} eq 'struct')
  {
     my $q={};
     foreach(sort keys %{$types{$p->{'type'}}->{'list'}})
     {
       my $t=$_;
       $types{$p->{'type'}}->{'list'}->{$t};
       my $z=generate_type_valid($types{$p->{'type'}}->{'list'}->{$t});
       $q->{$t}=$z if defined $z;
     }
     return $q;
  }

  die 'unknown tag: '.Dumper $p->{'type'};
}

sub generate_valid
{
  my $method=@table[int rand @table];
  my $rv={};


  foreach(@{$method->[2]})
  {
    next if exists $_->{'mandatory'} and (int rand 2)==0;
    my $t=generate_type_valid($_);
    $rv->{$_->{'name'}} = $t if defined $t;
  }
 
  return { $method->[1] => { 'name' => $method->[0] , 'correlationID' => int rand 100000, 'parameters' => $rv}};
}

my $iters=$config{'loops'};

$iters+=0;
$iters=10 if $iters==0;

goto dryrun if $config{'mode'} eq 'dryrun';

my $log=$config{'log'};
$log='fuzzer.log' if $log eq '';
unlink $log;

my $name=tmpnam();

`date >>$log`;
`ddate >>$log`;
#`echo $rnd >>$log`;

open(LOG,'>>',$log) or die;
LOG->autoflush(1);
printf LOG "random base %s\n",$rnd;

my $json;
my $err=0;
my $cnt=0;

my $json_xs=JSON::XS->new->utf8;

my $tpath=$config{'fuzzer'};

while($iters--)
{
  printf STDERR "%u remaining\n",$iters;
  open(FO, "|valgrind --tool=memcheck --leak-check=full --num-callers=24 --show-reachable=yes --track-fds=yes $tpath/alrpctest 2>&1 | grep -v DWARF >$name ; echo \"--finish--\" >>$name" )
        or die "cant open";
  FO->autoflush(1);
  $cnt++;
#  $json_xs->encode($perl_data);
#  $json=encode_json(generate_valid());
  $json=$json_xs->encode(generate_valid());
  printf FO "%s\n",$json;
  close FO;
  my $res;
  do
  {
    $res=read_file($name);
  } unless($res=~/--finish--$/s);

#'ERROR SUMMARY: 0 errors from 0 contexts'
#'--error--'
  if($res=~/\n--error--\n/s or $res!~/ERROR SUMMARY: 0 errors from 0 contexts/s)
  {
    $err++;
    printf LOG "::SENT\n%s\n",$json;
    printf LOG "::RECEIVED\n%s\n",$res;
  }
}

printf LOG "\n\ntotal runs %u, errors %u, random base %u\n\n",$cnt,$err,$rnd;
close LOG;

unlink $name;

printf STDERR "total runs %u, errors %u, random base %u\n\n",$cnt,$err,$rnd;

exit 0;

dryrun:

$log=$config{'dryrun'};

my $json_xs=JSON::XS->new->utf8;

open(FO, '>', $log) or die "cant open";
FO->autoflush(1);

printf FO "%s\n",$json_xs->encode(generate_valid()) while $iters--;
close FO;

exit 0;

generate:

# dependencies checking
# i think it will be safer skip dependency checking. may be later when Moon moved in other constellation. today is bad time, really. :-)


# preparing

my $path=$config{'generate'};
$path=~s/\/$//;
$path.='/';

my $pver=exists $config{'pver'} ? $config{'pver'} : 1;
my $pimpl=$config{'pimpl'};
$pimpl=~s/\/$//;
$pimpl.='/';

my $lib=$config{'lib'};
$lib ='lib' if $lib eq '';
$lib=~s/\/$//;
$lib.='/';

my $p2i=File::Spec->abs2rel($path,$pimpl);
my $i2p=File::Spec->abs2rel($pimpl,$path);
my $i2l=File::Spec->abs2rel($lib,$path);

$p2i=$path if exists $config{'abs'};
$i2p=$pimpl if exists $config{'abs'};
$i2l=$lib if exists $config{'abs'};

$p2i=~s/\/$//;
$i2p=~s/\/$//;
$i2l=~s/\/$//;


my $ns=$config{'ns'};
my $ind= $ns eq '' ? '' : '  ';

my $sh=exists $config{'sh'} ? $config{'sh'} : 'hh';
my $sc=exists $config{'sc'} ? $config{'sc'} : 'cc';

`mkdir -p $path`;
`mkdir -p $pimpl`;

my $timestamp=gmtime(time());

my $pragma='';
#$pragma = sprintf "#pragma GCC dependency \"%s\"\n\n",File::Spec->canonpath($config{'xml'}) if exists $config{'pragma'};
$pragma = sprintf "#pragma GCC dependency \"%s\"\n\n",Cwd::realpath($config{'xml'}) if exists $config{'pragma'};

my $timemod;

{
  my @a=stat $config{'xml'};
  $timemod=gmtime(@a[9]);
  undef @a;
}

sub getfort
{
  my $x=`fortune -s`;

  $x=~s/\*\//* \//gs;
  $x=~s/\/\*/\/ \*/gs;
  return sprintf "/*\n%s*/\n",$x;
}

my $hat=sprintf "\n/*\n  interface\t%s\n  version\t%s\n  date\t\t%s\n  generated at\t%s\n  source stamp\t%s\n  author\trobok0der\n*/\n\n",
        $exts{'name'},$exts{'version'},$exts{'date'},$timestamp,$timemod;

my $dd=`ddate`;
$dd=~s/\n/ /gs;
$hat= sprintf "// %s%s",$dd,$hat if exists $config{'ee'};

my $prefix=$ns eq '' ? "" : sprintf "%s_",uc $ns;


sub header_h
{
  my $name=shift;

  return sprintf "%s#ifndef %s%s_INCLUDE\n#define %s%s_INCLUDE\n\n",
          $pragma,
          $prefix,uc $name,$prefix,uc $name;
}


sub comment2
{
  my $x=shift;
  return '' if $x eq '';
  return sprintf "\n///  %s\n",$x if not ref $x;
  return sprintf "\n///  %s\n",$x->[0] if @$x==1;
  return sprintf "\n/**\n     %s\n*/\n",join("\n     ",@$x);
}

sub comment
{
  my $x=shift;
  return '' if not ref $x;
  return '' if not exists $x->{'descr'};
  my $d=join("\n     ",@{$x->{'descr'}});
  return @{$x->{'descr'}}>1 ? sprintf "\n/**\n     %s\n*/\n",$d : sprintf "\n///  %s\n",$d;
}

sub mkpath
{
  my $x=shift;
  my $s=shift;
#  $x=$path.'/'.$x.'.'.$s;
  $x.='.'.$s;
  return '>'.$x if not exists $config{'style'};
  return sprintf "| %s > %s",$config{'style'},$x;
}

open(FO,mkpath($pimpl.'PerfectHashTable',$sh)) or die;
print FO "#ifndef PERFECTHASHTABLE_INCLUDE\n#define PERFECTHASHTABLE_INCLUDE\n\n";

print FO "struct PerfectHashTable\n{\n  const char *name;\n  unsigned int idx;\n};\n" if $ns eq '';
printf FO "namespace %s\n{\n  struct PerfectHashTable\n  {\n    const char *name;\n    unsigned int idx;\n  };\n}",$ns if $ns ne '';

print FO "\n#endif\n";
close FO;

# enums processing, 4 files per type produced.
foreach(sort keys %types)
{
  my $k=$_;
  next if $types{$k}->{'type'} ne 'enum';
  my $cmt=comment($types{$k});

  open(FO,mkpath($path.$k,$sh)) or die;
  print FO header_h($k);
  print FO $hat;

  my $templ_enum='
class #name#
{
public:
  enum #name#Internal
  {
    INVALID_ENUM=-1'
;
#    EN_US=0,
  my $templ_enum2='
  };

  #name#() : mInternal(INVALID_ENUM)				{}
  #name#(#name#Internal e) : mInternal(e)		{}

  #name#Internal get(void) const	{ return mInternal; }
  void set(#name#Internal e)		{ mInternal=e; }

private:
  #name#Internal mInternal;
  friend class #name#Marshaller;
};
';
  printf FO "namespace %s\n{\n",$ns if $ns ne '';
  print FO $cmt;
  my $t=$templ_enum;
  $t=~s/\n/\n  /gs if $ns ne '';
  $t=~s/#name#/$k/gs;
  print FO $t;
  
#  die Dumper($types{$k}) if $k eq 'Language';
  
  for(my $i=0;$i<@{$types{$k}->{'list'}};$i++)
  {
    my $c='';
    $c=comment2($types{$k}->{'ldesc'}->[$i]) if defined $types{$k}->{'ldesc'}->[$i];
    printf FO ",\n";
    printf FO $c if $c ne '';
    my $t= defined $types{$k}->{'iname'}->[$i] ? $types{$k}->{'iname'}->[$i] : $types{$k}->{'list'}->[$i];
    printf FO "    %s%s=%u",$ind,$t,$i;
  }  
  $t=$templ_enum2;
  $t=~s/\n/\n  /gs if $ns ne '';
  $t=~s/#name#/$k/gs;
  print FO $t;
  print FO "\n}\n" if $ns ne '';
  print FO "\n#endif\n";
  close FO;

  open(FO,'>',sprintf "%s%sMarshaller.gp",$pimpl,$k) or die;

  print FO "%language=C++\n%struct-type\n%define lookup-function-name getPointer\n%define class-name ".$k."_intHash\n%omit-struct-type\n";
  print FO "%readonly-tables\n%compare-strncmp\n%compare-lengths\n\nstruct ";
  printf FO "%s::",$ns if $ns ne '';
  print FO "PerfectHashTable\n{\n  const char *name;\n  unsigned int idx;\n};\n\n%%\n";

  for(my $i=0;$i<@{$types{$k}->{'list'}};$i++)
  {
    printf FO "%s,%u\n",$types{$k}->{'list'}->[$i],$i;
  }

  close FO;
  
  open(FO,mkpath(sprintf('%s%sMarshaller',$pimpl,$k),$sh)) or die;
  print FO header_h($k.'Marshaller');
  
#  printf FO "#include <string>\n#include <jsoncpp/json.h>\n\n#include \"PerfectHashTable.%s\"\n\n#include \"../%s.%s\"\n\n",$sh,$k,$sh;
  printf FO "#include <string>\n#include <jsoncpp/json.h>\n\n#include \"PerfectHashTable.%s\"\n\n#include \"%s/%s.%s\"\n\n",$sh,$p2i,$k,$sh;

  print FO $hat;
  printf FO "namespace %s\n{\n",$ns if $ns ne '';

  printf FO "\n//! marshalling class for %s\n",$k;

  my $templ='
class #name#Marshaller
{
public:

  static std::string toName(const #name#& e) 	{ return getName(e.mInternal) ?: ""; }

  static bool fromName(#name#& e,const std::string& s)
  { 
    return (e.mInternal=getIndex(s.c_str()))!=#name#::INVALID_ENUM;
  }

  static bool checkIntegrity(#name#& e)		{ return e.mInternal!=#name#::INVALID_ENUM; } 
  static bool checkIntegrityConst(const #name#& e)	{ return e.mInternal!=#name#::INVALID_ENUM; } 

  static bool fromString(const std::string& s,#name#& e);
  static const std::string toString(const #name#& e);

  static bool fromJSON(const Json::Value& s,#name#& e);
  static Json::Value toJSON(const #name#& e);

  static const char* getName(#name#::#name#Internal e)
  {
     return (e>=0 && e<#num#) ? mHashTable[e].name : NULL;
  }

  static const #name#::#name#Internal getIndex(const char* s);

  static const PerfectHashTable mHashTable[#num#];
};
';
  my $z=$templ;
  $z=~s/\n/\n  /gs if $ns ne '';
  $z=~s/#name#/$k/gs;
  my $t=@{$types{$k}->{'list'}};
  $z=~s/#num#/$t/gs;
   
  print FO $z;

  print FO "\n}\n" if $ns ne '';

  print FO "\n#endif\n";
  close FO;

# enumerator marshaller cc file

  open(FO,mkpath(sprintf('%s%sMarshaller',$pimpl,$k),$sc)) or die;
  print FO $pragma;
  my $nsc= $ns eq '' ? '' : "using namespace $ns;\n";
  my $hhat=$hat;
  {
    if(exists $config{'ee'})
    {
      $hhat=~s/^\/\/.*?\n//s;
      $hhat=getfort().$hhat;
    }
  }

  
$templ='#include <cstring>
#include "#p2i#/#name#.#sh#"
#include "#name#Marshaller.#sh#"
#include "#name#Marshaller.inc"

'.$hhat.$nsc.'

const #name#::#name#Internal #name#Marshaller::getIndex(const char* s)
{
  if(!s)
    return #name#::INVALID_ENUM;
  const struct PerfectHashTable* p=#name#_intHash::getPointer(s,strlen(s));
  return p ? static_cast<#name#::#name#Internal>(p->idx) : #name#::INVALID_ENUM;
}


bool #name#Marshaller::fromJSON(const Json::Value& s,#name#& e)
{
  e.mInternal=#name#::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=#name#::INVALID_ENUM);
}


Json::Value #name#Marshaller::toJSON(const #name#& e)
{
  if(e.mInternal==#name#::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool #name#Marshaller::fromString(const std::string& s,#name#& e)
{
  e.mInternal=#name#::INVALID_ENUM;
  try
  {
    Json::Reader reader;
    Json::Value json;
    if(!reader.parse(s,json,false))  return false;
    if(fromJSON(json,e))  return true;
  }
  catch(...)
  {
    return false;
  }
  return false;
}

const std::string #name#Marshaller::toString(const #name#& e)
{
  Json::FastWriter writer;
  return e.mInternal==#name#::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable #name#Marshaller::mHashTable[#num#]=
{
';

  $z=$templ;
  $z=~s/#name#/$k/gs;
  $z=~s/#num#/$t/gs;
  $z=~s/#sh#/$sh/gs;
  $z=~s/#p2i#/$p2i/gs;

  print FO $z;

  my @r;
  for(my $i=0;$i<@{$types{$k}->{'list'}};$i++)
  {
    push @r,sprintf "  {\"%s\",%u}",$types{$k}->{'list'}->[$i],$i;
  }

  print FO join(",\n",@r);

  print FO "\n};\n";
  close FO;
}

# ooh, enums processed, going to variants. it's more complex.
# some stuff

sub prepare_field
{
  my $n=shift;
  my $v=shift;

  my $r={ 'name' => $n, 'type' => $v->{'type'}};
  $r->{'ename'}= $v->{'ename'} if exists $v->{'ename'};
  $r->{'descr'}= $v->{'description'} if exists $v->{'description'};

  if(exists $v->{'array'})
  {
    $r->{'array'}=[];
    $r->{'array'}->[0]=$v->{'minsize'} if exists $v->{'minsize'};
    $r->{'array'}->[1]=$v->{'maxsize'} if exists $v->{'maxsize'};
  }
  $r->{'opt'}=1 if exists $v->{'mandatory'};

  if($v->{'type'} eq 'String')
  {
      $r->{'ctype'}='std::string';
      $r->{'ref'}=1;
      $r->{'max'}=$v->{'maxLength'} if exists $v->{'maxLength'};
      $r->{'max'}=$v->{'maxlength'} if exists $v->{'maxlength'};
      return $r;
  }
  if($v->{'type'} eq 'Float')
  {
      $r->{'ctype'}='float';
      $r->{'ref'}=0;
      $r->{'max'}=$v->{'maxvalue'} if exists $v->{'maxvalue'};
      $r->{'min'}=$v->{'minvalue'} if exists $v->{'minvalue'};
      return $r;
  }
  if($v->{'type'} eq 'Boolean')
  {
    $r->{'ctype'}='bool';
    $r->{'ref'}=0;
    return $r;
  }

  if($v->{'type'} eq 'Integer')
  {
    $r->{'ref'}=0;
    $r->{'max'}=$v->{'maxvalue'} if exists $v->{'maxvalue'};
    $r->{'min'}=$v->{'minvalue'} if exists $v->{'minvalue'};
    $r->{'ctype'}=( exists $r->{'min'} and $r->{'min'}>=0) ? 'unsigned int' : 'int';
    return $r;
  }

#  die sprintf "unknown type found in %s : %s",$k,$v->{'type'} if not exists $types{$v->{'type'}};
  return undef if not exists $types{$v->{'type'}};
  $r->{'ctype'}=$v->{'type'};
  $r->{'ref'}=1;
  return $r;
}

# 1. field list preparing
sub prepare_fields
{
  my $k=shift;
  my $rv=[[],0,0];
  
  foreach(sort keys %{$types{$k}->{'list'}})
  {
    my $n=$_;
    my $v=$types{$k}->{'list'}->{$n};
    
    my $z=prepare_field($n,$v);
    die sprintf "unknown type found in %s : %s",$k,$v->{'type'} if not defined $z;
    push @{$rv->[0]},prepare_field($n,$v);
    $rv->[2]=1 if exists $v->{'array'};
    $rv->[1]=1 if $v->{'type'} eq 'String';
  }
  
  return $rv;
}


sub declare_field
{
  my $r=shift;
  my $rv='';
  
  my $o= exists $r->{'opt'} ? '*' : '';

  $rv= exists $r->{'array'} ?  sprintf "std::vector<%s>%s %s;",$r->{'ctype'},$o,$r->{'name'} : sprintf "%s%s %s;",$r->{'ctype'},$o,$r->{'name'};

  my $cms='';

  
  $cms.="  [%s..%s] ",$r->{'array'}->[0],$r->{'array'}->[1] if exists $r->{'array'};

  $cms.=sprintf "(%s)",$r->{'max'} if $r->{'ctype'} eq 'std::string' and exists $r->{'max'};
  if($r->{'type'} eq 'Float' or $r->{'type'} eq 'Integer')
  {
     my $min;$min=$r->{'min'} if exists $r->{'min'};
     my $max;$max=$r->{'max'} if exists $r->{'max'};
     $cms.=sprintf " (%s,%s)",$min,$max if $max ne '' and $min ne '';
   }
   
  $rv.="\t//!< ".$cms if $cms ne '';

  return $rv;
}

sub declare_field2
{
  my $r=shift;
  my $rv=declare_field($r);
  my $c='';
  $c=comment2($r->{'descr'}) if exists $r->{'descr'};
#  print Dumper($c.$rv,$r);
  return $c.$rv;
}

sub declare_setter
{
  my $r=shift;
  my $rv='';
  
  return sprintf "bool set_%s(%s %s_);",$r->{'name'},$r->{'ctype'},$r->{'name'}
    if(($r->{'type'} eq 'Float' or $r->{'type'} eq 'Integer' or $r->{'type'} eq 'Boolean') and not exists $r->{'array'});
  
  $rv= exists $r->{'array'} ? sprintf "std::vector<%s>",$r->{'ctype'} : $r->{'ctype'};

  return sprintf "bool set_%s(const %s& %s_);",$r->{'name'},$rv,$r->{'name'};
}


sub define_setter
{
  my $n=shift;
  my $p=shift;				# parent name
  my $t=shift;				# 0 for variants and 1 for structs

  my $rv=declare_setter($n);
  $rv=~s/^bool //s;
  $rv=~s/;$//s;
  $rv=sprintf "bool %s::%s\n{\n",$p,$rv;

  my $f= $t==1 ? $n->{'name'} : ('mInternal.'.$n->{'name'});

  my $tp= exists $n->{'array'} ? sprintf "std::vector<%s>",$n->{'ctype'} : $n->{'ctype'};

# check constraints

  if(defined $n->{'array'})
  {
     $rv.= sprintf "  unsigned int i=%s_.size();\n",$n->{'name'};
     if(defined $n->{'array'}->[0] and defined $n->{'array'}->[1])
     {
       $rv.= sprintf "  if(i>%s || i<%s)  return false;\n",$n->{'array'}->[1],$n->{'array'}->[0];
     }
     else
     {
       $rv.= sprintf "  if(i<%s) return false;\n",$n->{'array'}->[0] if defined $n->{'array'}->[0];
       $rv.= sprintf "  if(i>%s) return false;\n",$n->{'array'}->[1] if defined $n->{'array'}->[1];
     }

    $rv.="  while(i--)\n  {\n" if $n->{'type'} ne 'Boolean';
    
    if($n->{'type'} eq 'String')
    {
      $rv.=sprintf "    if(%s_[i].length()>%s)  return false;\n",$n->{'name'},$n->{'max'} if exists $n->{'max'};
    }
    elsif($n->{'type'} eq 'Integer')
    {
      $rv.=sprintf "    if(%s_[i]>%s)  return false;\n",$n->{'name'},$n->{'max'} if exists $n->{'max'};
      $rv.=sprintf "    if(%s_[i]<%s)  return false;\n",$n->{'name'},$n->{'min'} if exists $n->{'min'} and $n->{'ctype'} eq 'unsigned int' and $n->{'min'}>0;
    }
    elsif($n->{'type'} eq 'Float')
    {
      $rv.=sprintf "    if(%s_[i]>%s)  return false;\n",$n->{'name'},$n->{'max'} if exists $n->{'max'};
      $rv.=sprintf "    if(%s_[i]<%s)  return false;\n",$n->{'name'},$n->{'min'} if exists $n->{'min'};
    }
    elsif($n->{'type'} ne 'Boolean')
    {
       $rv.=sprintf "    if(!%sMarshaller::checkIntegrityConst(%s_[i]))   return false;\n",$n->{'ctype'},$n->{'name'};
    }
    $rv.="  }\n" if $n->{'type'} ne 'Boolean';
  }
  else
  {
    if($n->{'type'} eq 'String')
    {
      $rv.=sprintf "  if(%s_.length()>%s)  return false;\n",$n->{'name'},$n->{'max'} if exists $n->{'max'};
    }
    elsif($n->{'type'} eq 'Integer')
    {
      $rv.=sprintf "  if(%s_>%s)  return false;\n",$n->{'name'},$n->{'max'} if exists $n->{'max'};
      $rv.=sprintf "  if(%s_<%s)  return false;\n",$n->{'name'},$n->{'min'} if exists $n->{'min'} and $n->{'ctype'} eq 'unsigned int' and $n->{'min'}>0;
    }
    elsif($n->{'type'} eq 'Float')
    {
      $rv.=sprintf "  if(%s_>%s)  return false;\n",$n->{'name'},$n->{'max'} if exists $n->{'max'};
      $rv.=sprintf "  if(%s_<%s)  return false;\n",$n->{'name'},$n->{'min'} if exists $n->{'min'};
    }
    elsif($n->{'type'} ne 'Boolean')
    {
       $rv.=sprintf "  if(!%sMarshaller::checkIntegrityConst(%s_))   return false;\n",$n->{'ctype'},$n->{'name'};
    }
  }

  if(exists $n->{'opt'})
  {
    $rv.=sprintf "  delete %s;\n  %s=0;\n\n  %s=new %s(%s_);\n",$f,$f,$f,$tp,$n->{'name'};
  }
  else
  {
    $rv.=sprintf "  mInternal.mDiscriminator=%s;\n", $n->{'ename'} if $t==0;

    if(exists $n->{'opt'})
    {
      $rv.=sprintf "  %s=new %s(%s_);\n",$f,$tp,$n->{'name'};
    }
    else
    {
      $rv.=sprintf "  %s=%s_;\n",$f,$n->{'name'};
    }
  }

  $rv.="  return true;\n}\n\n";

  return $rv;
}


# variants


foreach(sort keys %types)
{
  my $k=$_;
  next if $types{$k}->{'type'} ne 'variant';
  
  my $cmt=comment($types{$k});

  open(FO,mkpath($path.$k,$sh)) or die;
  print FO header_h($k);
  
  my $t=0+keys %{$types{$k}->{'list'}};
  
  my $fields=prepare_fields($k);
  my @tp=@{$fields->[0]};
  my $isstr=$fields->[1];
  my $isarr=$fields->[2];

  print FO "#include <string>\n\n" if $isstr==1;
  print FO "#include <vector>\n\n" if $isarr==1;

  foreach(@tp)
  {
    next if $_->{'ref'}==0;
    next if $_->{'ctype'} eq 'std::string';
    printf FO "#include \"%s.%s\"\n",$_->{'ctype'},$sh;
  }

  print FO "\n",$hat;
  print FO "\n";
  printf FO "namespace %s\n{\n",$ns if $ns ne '';
  print FO $cmt;

  my $tmpl=sprintf "class %s\n{\npublic:\n\n  enum eDiscriminator\n  {\n    INVALID_ENUM=-1",$k;
  $tmpl=~s/\n/\n  /gs if $ns ne '';
  printf FO $tmpl;

  for(my $i=0;$i<@tp;$i++)
  {
    printf FO ",\n    %s%s=%u",$ind, @tp[$i]->{'ename'},$i;
  }
  printf FO "\n  %s};\n\n  %sstruct tUnion\n%s  {\n%s    eDiscriminator mDiscriminator;\n",$ind,$ind,$ind,$ind;
 
 foreach(@tp)
 {
   my $n=$_;
   my $c='';
   $c=comment2($n->{'descr'}) if exists $n->{'descr'};
#   printf FO "%s%s    %s\n",$c,$ind,declare_field($_)  foreach @tp;
   printf FO "%s%s    %s\n",$c,$ind,declare_field($n);
 } 
 
  printf FO "%s  };\n",$ind;
  
  my $tmpl='
  #name#(const #name#& c);
  #name#(void);

  ~#name#(void);

//! return true if all constraints valid
  bool checkIntegrity(void);

// getters
  eDiscriminator getType(void);
  eDiscriminator get_All(tUnion& t);
//setters

';
  $tmpl=~s/#name#/$k/gs;
  $tmpl=~s/\n /\n   /gs if $ns ne '';
  print FO $tmpl;
  
  foreach(@tp)
  {
    printf FO "%s  %s\n",$ind,declare_setter($_);
  }

  $tmpl="\n  bool set_All(const tUnion& t);\n\nprivate:\n  tUnion mInternal;\n\n  friend class #name#Marshaller;\n";
  $tmpl=~s/#name#/$k/gs;
  $tmpl=~s/\n /\n   /gs if $ns ne '';
  print FO $tmpl;
  
  
  printf FO "%s};\n",$ind;
  print FO "\n}\n" if $ns ne '';
  print FO "\n#endif\n";
  close FO;

  open(FO,'>',sprintf "%s%sMarshaller.gp",$pimpl,$k) or die;
  print FO "%language=C++\n%struct-type\n%define lookup-function-name getPointer\n%define class-name ".$k."_intHash\n%omit-struct-type\n";
  print FO "%readonly-tables\n%compare-strncmp\n%compare-lengths\n\nstruct ";
  printf FO "%s::",$ns if $ns ne '';
  print FO "PerfectHashTable\n{\n  const char *name;\n  unsigned int idx;\n};\n\n%%\n";
  
  for(my $i=0;$i<@tp;$i++)
  {
    printf FO "%s,%u\n", @tp[$i]->{'ename'},$i;
  }

  close FO;
  
  open(FO,mkpath($pimpl.$k,$sc)) or die;
  print FO $pragma;
  print FO "#include <cstring>\n\n";
  printf FO "#include \"%s/%s.%s\"\n",$p2i,$k,$sh;

  {
    my %h = map { $_->{'ctype'} ,1} grep { $_->{'ref'}==1 and $_->{'ctype'} ne 'std::string' } @tp;
    print FO join("\n",map { sprintf '#include "%sMarshaller.%s"',$_,$sh } sort keys %h);
  }

  printf FO "\n#include \"%sMarshaller.%s\"\n\n",$k,$sh;

  {
    my $h=$hat;
    if(exists $config{'ee'})
    {
      $h=~s/^\/\/.*?\n//s;
      $h=getfort().$h;
    }
    print FO $h;
  }
  
  printf FO "using namespace %s;\n",$ns if $ns ne '';

  $tmpl='
#name#::~#name#(void)
{
}

#name#::#name#(const #name#& c)
{
  *this=c;
}

#name#::#name#(void)
{
  mInternal.mDiscriminator=INVALID_ENUM;
}


bool #name#::checkIntegrity(void)
{
  return #name#Marshaller::checkIntegrity(*this); 
}

#name#::eDiscriminator #name#::getType(void)
{
  return mInternal.mDiscriminator;
}

#name#::eDiscriminator #name#::get_All(tUnion& t)
{
  t=mInternal;
  return mInternal.mDiscriminator;
}

bool #name#::set_All(const tUnion& t)
{
  mInternal=t;
  return checkIntegrity();
}
';
  
  my $z=$tmpl;
  $z=~s/#name#/$k/gs;
  print FO $z;

# variant setters
  print FO define_setter($_,$k,0) foreach @tp;

  close FO;

# marshaller hh

  open(FO,mkpath(sprintf('%s%sMarshaller',$pimpl,$k),$sh)) or die;

  print FO header_h($k.'Marshaller');

  printf FO "#include <string>\n#include <jsoncpp/json.h>\n\n#include \"PerfectHashTable.%s\"\n#include \"%s/%s.%s\"\n\n",
        $sh,$p2i,$k,$sh;

  print FO $hat;

  printf FO "namespace %s\n{\n",$ns if $ns ne '';

  $tmpl='
struct #name#Marshaller
{
  static std::string toName(const #name#& e) 	{ return getName(e.mInternal.mDiscriminator) ?: ""; }

  static bool fromName(#name#::eDiscriminator& e,const std::string& s)
  { 
    return (e=getIndex(s.c_str()))!=#name#::INVALID_ENUM;
  }

  static bool checkIntegrity(#name#& e);
  static bool checkIntegrityConst(const #name#& e);

  static bool fromString(const std::string& s,#name#& e);
  static const std::string toString(const #name#& e);

  static bool fromJSON(const Json::Value& s,#name#& e);
  static Json::Value toJSON(const #name#& e);


  static const char* getName(#name#::eDiscriminator e)
  {
     return (e>=0 && e<#num#) ? mHashTable[e].name : NULL;
  }
   
  static const #name#::eDiscriminator getIndex(const char* s);

  static const PerfectHashTable mHashTable[#num#];
};';

  my $z=$tmpl;
  $z=~s/\n/\n  /gs if $ns ne '';
  $z=~s/#name#/$k/gs;
  my $t=scalar keys %{$types{$k}->{'list'}};
  $z=~s/#num#/$t/gs;
  print FO $z;

  printf FO "\n}\n" if $ns ne '';
  print FO "\n#endif\n";
   
  close FO;

# long way to finish, i want COFFEE!!!
# but last variant file must be done before..

  open(FO,mkpath(sprintf('%s%sMarshaller',$pimpl,$k),$sc)) or die;
  print FO $pragma;

  my $nsc= $ns eq '' ? '' : "using namespace $ns;\n\n";
  my $hhat=$hat;
  {
    if(exists $config{'ee'})
    {
      $hhat=~s/^\/\/.*?\n//s;
      $hhat=getfort().$hhat;
    }
  }

  print FO "#include <cstring>\n\n";

  printf FO "#include \"%s/%s.%s\"\n",$p2i,$k,$sh;

  {
    my %h = map { $_->{'ctype'} ,1} grep { $_->{'ref'}==1 and $_->{'ctype'} ne 'std::string' } @tp;
    print FO join("\n",map { sprintf '#include "%sMarshaller.%s"',$_,$sh } sort keys %h);
  }
  
  printf FO "\n\n#include \"%sMarshaller.%s\"\n#include \"%sMarshaller.inc\"\n\n",$k,$sh,$k;

  print FO $hhat;
  
  print FO $nsc;

  $tmpl='bool #name#Marshaller::checkIntegrity(#name#& s)
{
  return checkIntegrityConst(s);
}

const #name#::eDiscriminator #name#Marshaller::getIndex(const char* s)
{
  if(!s)
    return #name#::INVALID_ENUM;
  const struct PerfectHashTable* p=#name#_intHash::getPointer(s,strlen(s));
  return p ? static_cast<#name#::eDiscriminator>(p->idx) : #name#::INVALID_ENUM;
}


bool #name#Marshaller::fromString(const std::string& s,#name#& e)
{
  try
  {
    Json::Reader reader;
    Json::Value json;
    if(!reader.parse(s,json,false))  return false;
    if(!fromJSON(json,e))  return false;
  }
  catch(...)
  {
    return false;
  }
  return true;
}


const std::string #name#Marshaller::toString(const #name#& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}

';

  $tmpl=~s/#name#/$k/gs;
  print FO $tmpl;

# and there are four untemplated methods.
# my coffee wait me

  printf FO "\nbool %sMarshaller::checkIntegrityConst(const %s& s)\n{\n  switch(s.mInternal.mDiscriminator)\n  {\n".
                "    case %s::INVALID_ENUM:\n      return false;\n",$k,$k,$k;

   foreach(@tp)
   {
     my $n=$_;
     printf FO "    case %s::%s:\n",$k, $n->{'ename'};

    if(defined $n->{'array'})
    {
       printf FO "    {\n      unsigned int i=s.mInternal.%s.size();\n",$n->{'name'};
       if(defined $n->{'array'}->[0] and defined $n->{'array'}->[1])
       {
         printf FO "      if(i>%s || i<%s)  return false;\n",$n->{'array'}->[1],$n->{'array'}->[0];
       }
       else
       {
         printf FO "      if(i<%s) return false;\n",$n->{'array'}->[0] if defined $n->{'array'}->[0];
         printf FO "      if(i>%s) return false;\n",$n->{'array'}->[1] if defined $n->{'array'}->[1];
       }

      print FO "      while(i--)\n      {\n";
    
      if($n->{'type'} eq 'String')
      {
        printf FO "        if(s.mInternal.%s[i].length()>%s)  return false;\n",$n->{'name'},$n->{'max'} if exists $n->{'max'};
      }
      elsif($n->{'type'} eq 'Integer')
      {
        printf FO "        if(s.mInternal.%s[i]>%s)  return false;\n",$n->{'name'},$n->{'max'} if exists $n->{'max'};
        printf FO "        if(s.mInternal.%s[i]<%s)  return false;\n",$n->{'name'},$n->{'min'} if exists $n->{'min'} and $n->{'ctype'} eq 'unsigned int' and $n->{'min'}>0;
      }
      elsif($n->{'type'} eq 'Float')
      {
        printf FO "        if(s.mInternal.%s[i]>%s)  return false;\n",$n->{'name'},$n->{'max'} if exists $n->{'max'};
        printf FO "        if(s.mInternal.%s[i]<%s)  return false;\n",$n->{'name'},$n->{'min'} if exists $n->{'min'};
      }
      elsif($n->{'type'} ne 'Boolean')
      {
         printf FO "        if(!%sMarshaller::checkIntegrityConst(s.mInternal.%s[i]))   return false;\n",$n->{'ctype'},$n->{'name'};
      }
       printf FO "    }\n    return true;\n";
     }
     else
     {
      if($n->{'type'} eq 'String')
       {
         printf FO "      if(s.mInternal.%s.length()>%s)  return false;\n",$n->{'name'},$n->{'max'} if exists $n->{'max'};
       }
       elsif($n->{'type'} eq 'Integer')
       {
         printf FO "      if(s.mInternal.%s>%s)  return false;\n",$n->{'name'},$n->{'max'} if exists $n->{'max'};
         printf FO "      if(s.mInternal.%s<%s)  return false;\n",$n->{'name'},$n->{'min'} if exists $n->{'min'} and $n->{'ctype'} eq 'unsigned int' and $n->{'min'}>0;
       }
       elsif($n->{'type'} eq 'Float')
       {
         printf FO "      if(s.mInternal.%s>%s)  return false;\n",$n->{'name'},$n->{'max'} if exists $n->{'max'};
         printf FO "      if(s.mInternal.%s<%s)  return false;\n",$n->{'name'},$n->{'min'} if exists $n->{'min'};
       }
       elsif($n->{'type'} ne 'Boolean')
       {
          printf FO "      return %sMarshaller::checkIntegrityConst(s.mInternal.%s);\n",$n->{'ctype'},$n->{'name'};
          next;
       }
       printf FO "      return true;\n";
     }
     
   }

  printf FO "  }\n  return false;\n}\n";
 
  printf FO "\nJson::Value %sMarshaller::toJSON(const %s& e)\n{\n  Json::Value json(Json::objectValue);\n  ".
           "if(!checkIntegrityConst(e))\n    return Json::Value(Json::nullValue);\n\n  switch(e.mInternal.mDiscriminator)\n  ".
           "{\n    case %s::INVALID_ENUM:\n      break;\n\n",$k,$k,$k;


  foreach (@tp)
  {
    my $n=$_;
    printf FO "    case %s::%s:\n",$k,$n->{'ename'};
    if(defined $n->{'array'})
    {
      printf FO "      json[\"%s\"]=Json::Value(Json::arrayValue);\n      json[\"%s\"].resize(e.mInternal.%s.size());\n",
        $n->{'name'},$n->{'name'},$n->{'name'};
      printf FO "      for(unsigned int i=0;i<e.mInternal.%s.size();i++)\n        json[\"%s\"][i]=",$n->{'name'},$n->{'name'};
      if($n->{'type'} eq 'Boolean' or $n->{'type'} eq 'Float' or $n->{'type'} eq 'Integer' or $n->{'type'} eq 'String')
      {
        printf FO "Json::Value(e.mInternal.%s[i]);\n",$n->{'name'};
      }
      else
      {
        printf FO "%sMarshaller::toJSON(e.mInternal.%s[i]);\n",$n->{'type'},$n->{'name'};
      }
      print FO "      break;\n";
      next;
    }

    printf FO "      json[\"%s\"]=",$n->{'name'};

    if($n->{'type'} eq 'Boolean' or $n->{'type'} eq 'Float' or $n->{'type'} eq 'Integer' or $n->{'type'} eq 'String')
    {
      printf FO "Json::Value(e.mInternal.%s);\n",$n->{'name'};
    }
    else
    {
      printf FO "%sMarshaller::toJSON(e.mInternal.%s);\n",$n->{'type'},$n->{'name'};
    }
    print FO "      break;\n";
  }

  print FO "  }\n\n  return json;\n}\n\n";

  printf FO "bool %sMarshaller::fromJSON(const Json::Value& json,%s& c)\n{\n  c.mInternal.mDiscriminator=%s::INVALID_ENUM;\n\n".
            "  try\n  {\n    if(!json.isObject())  return false;\n\n",$k,$k,$k;


  foreach(@tp)
  {
    my $n=$_;
    printf FO "    if(json.isMember(\"%s\"))\n    {\n",$n->{'name'};
    if(exists $n->{'array'})
    {
      printf FO "      const Json::Value& j=json[\"%s\"];\n",$n->{'name'};
      printf FO "      if(!j.isArray())  return false;\n";
      printf FO "      c.mInternal.%s.resize(j.size());\n",$n->{'name'};
      printf FO "      for(unsigned int i=0;i<j.size();i++)\n";
      
      if(exists $json{$n->{'type'}})
      {
        printf FO "        if(!j[i].is%s())\n",$json2{$n->{'type'}};
        printf FO "          return false;\n";
        printf FO "        else\n";
        printf FO "          c.mInternal.%s[i]=j[i].as%s();\n",$n->{'name'},$json{$n->{'type'}};
      }
      else
      {
        printf FO "        {\n";
        printf FO "          %s t;\n",$n->{'type'};
        printf FO "          if(!%sMarshaller::fromJSON(j[i],t))\n",$n->{'type'};
        printf FO "            return false;\n";
        printf FO "          c.mInternal.%s[i]=t;\n",$n->{'name'};
        printf FO "        }\n";
      }

      printf FO "      c.mInternal.mDiscriminator=%s::%s;\n      goto leave;\n    }\n",$k,$n->{'ename'};
      next;
    }
    else
    {
        printf FO "      const Json::Value& j=json[\"%s\"];\n",$n->{'name'};
      if(exists $json{$n->{'type'}})
      {
        printf FO "      if(!j.is%s())  return false;\n",$json2{$n->{'type'}};
        printf FO "      c.mInternal.%s=j.as%s();\n",$n->{'name'},$json{$n->{'type'}};
      }
      else
      {
        printf FO "      if(!%sMarshaller::fromJSON(j,c.mInternal.%s))\n",$n->{'type'},$n->{'name'};
        printf FO "        return false;\n";
      }
        printf FO "      c.mInternal.mDiscriminator=%s::%s;\n",$k, $n->{'ename'};
        printf FO "      goto leave;\n    }\n";
    }
  }

  printf FO "\n  }\n  catch(...)\n  {\n    c.mInternal.mDiscriminator=%s::INVALID_ENUM;\n    return false;\n  }\n".
  "  c.mInternal.mDiscriminator=%s::INVALID_ENUM;\n  return false;\nleave:\n  return checkIntegrity(c);\n}\n\n",$k,$k;


  printf FO "const PerfectHashTable %sMarshaller::mHashTable[%u]=\n{\n",$k,$t;

  my @r;
  for(my $i=0;$i<@tp;$i++)
  {
    push @r,sprintf "  {\"%s\",%u}", @tp[$i]->{'ename'},$i;
  }

  print FO join(",\n",@r);

  printf FO "\n};\n";

  close FO;
}

# my coffee ready...

# rock'n'roll!! structures!!!
# i hope interpreter not understood my comments :-)

sub declare_check
{
  my $n=shift;
  my $rv;

  my $w=$n->{'name'};
  $w.='[0]' if exists $n->{'opt'};

  if(defined $n->{'array'})
  {
     $rv.=sprintf "  if(s.%s)\n",$n->{'name'} if exists $n->{'opt'};
     $rv.=sprintf "  {\n    unsigned int i=s.%s.size();\n",$w;
     if(defined $n->{'array'}->[0] and defined $n->{'array'}->[1])
     {
       $rv.=sprintf "    if(i>%s || i<%s)  return false;\n",$n->{'array'}->[1],$n->{'array'}->[0];
     }
     else
     {
       $rv.=sprintf "    if(i<%s) return false;\n",$n->{'array'}->[0] if defined $n->{'array'}->[0];
       $rv.=sprintf "    if(i>%s) return false;\n",$n->{'array'}->[1] if defined $n->{'array'}->[1];
     }

     $rv.="    while(i--)\n    {\n";
    
    if($n->{'type'} eq 'String')
    {
      $rv.=sprintf "      if(s.%s[i].length()>%s)  return false;\n",$w,$n->{'max'} if exists $n->{'max'};
    }
    elsif($n->{'type'} eq 'Integer')
    {
      $rv.=sprintf "      if(s.%s[i]>%s)  return false;\n",$w,$n->{'max'} if exists $n->{'max'};
      $rv.=sprintf "      if(s.%s[i]<%s)  return false;\n",$w,$n->{'min'} if exists $n->{'min'} and $n->{'ctype'} eq 'unsigned int' and $n->{'min'}>0;
    }
    elsif($n->{'type'} eq 'Float')
    {
      $rv.=sprintf "    if(s.%s[i]>%s)  return false;\n",$w,$n->{'max'} if exists $n->{'max'};
      $rv.=sprintf "    if(s.%s[i]<%s)  return false;\n",$w,$n->{'min'} if exists $n->{'min'};
    }
    elsif($n->{'type'} ne 'Boolean')
    {
       $rv.=sprintf "    if(!%sMarshaller::checkIntegrityConst(s.%s[i]))   return false;\n",$n->{'ctype'},$w;
    }
     $rv.=sprintf "    }\n  }\n";
   }
   else
   {
    if($n->{'type'} eq 'String')
    {
      if(exists $n->{'opt'})
      {
        $rv.=sprintf "  if(s.%s && s.%s->length()>%s)  return false;\n",$n->{'name'},$n->{'name'},$n->{'max'} if exists $n->{'max'};
      }
      else
      {
        $rv.=sprintf "  if(s.%s.length()>%s)  return false;\n",$n->{'name'},$n->{'max'} if exists $n->{'max'};
      } 
    }
    elsif($n->{'type'} eq 'Integer')
    {
      if(exists $n->{'opt'})
      {
        $rv.=sprintf "  if(s.%s && *s.%s>%s)  return false;\n",$n->{'name'},$n->{'name'},$n->{'max'} if exists $n->{'max'};
        $rv.=sprintf "  if(s.%s && *s.%s<%s)  return false;\n",$n->{'name'},$n->{'name'},$n->{'min'} if exists $n->{'min'} and ($n->{'ctype'} ne 'unsigned int' or $n->{'min'}>0);
      }
      else
      {
        $rv.=sprintf "  if(s.%s>%s)  return false;\n",$n->{'name'},$n->{'max'} if exists $n->{'max'};
        $rv.=sprintf "  if(s.%s<%s)  return false;\n",$n->{'name'},$n->{'min'} if exists $n->{'min'} and ($n->{'ctype'} ne 'unsigned int' or $n->{'min'}>0);
      }
    }
    elsif($n->{'type'} eq 'Float')
    {
      if(exists $n->{'opt'})
      {
        $rv.=sprintf "  if(s.%s && *s.%s>%s)  return false;\n",$n->{'name'},$n->{'name'},$n->{'max'} if exists $n->{'max'};
        $rv.=sprintf "  if(s.%s && *s.%s<%s)  return false;\n",$n->{'name'},$n->{'name'},$n->{'min'} if exists $n->{'min'};
      }
      else
      {
        $rv.=sprintf "  if(s.%s>%s)  return false;\n",$n->{'name'},$n->{'max'} if exists $n->{'max'};
        $rv.=sprintf "  if(s.%s<%s)  return false;\n",$n->{'name'},$n->{'min'} if exists $n->{'min'};
      }
    }
    elsif($n->{'type'} ne 'Boolean')
    {
      if(exists $n->{'opt'})
      {
        $rv.=sprintf "  if(s.%s && !%sMarshaller::checkIntegrityConst(*s.%s))  return false;\n",$n->{'name'},$n->{'ctype'},$n->{'name'};
      }
      else
      {
        $rv.=sprintf "  if(!%sMarshaller::checkIntegrityConst(s.%s))  return false;\n",$n->{'ctype'},$n->{'name'};
      }
    }
 }
  return $rv;
}

sub declare_serialize
{
  my $n=shift;
  my $rv;

  if(defined $n->{'array'})
  {
    if(exists $n->{'opt'})
    {
      my $w="  if(e.#)\n  {\n    unsigned int sz=e.#->size();\n    json[\"#\"]=Json::Value(Json::arrayValue);\n    json[\"#\"].resize(sz);\n".
            "    for(unsigned int i=0;i<sz;i++)\n      json[\"#\"][i]=";

      if(exists $json{$n->{'type'}})
      {
         $w.="Json::Value(e.#[0][i]);\n  }\n";
      }
      else
      {
         $w.="#type#Marshaller::toJSON(e.#[0][i]);\n  }\n"
      }
      
      my $z=$n->{'type'};
      $w=~s/#type#/$z/gs;
      my $z=$n->{'name'};
      $w=~s/#/$z/gs;
      $rv.=$w;
    }
    else
    {
      $rv.=sprintf "  json[\"%s\"]=Json::Value(Json::arrayValue);\n  json[\"%s\"].resize(e.%s.size());\n",
        $n->{'name'},$n->{'name'},$n->{'name'};
      $rv.=sprintf "  for(unsigned int i=0;i<e.%s.size();i++)\n    json[\"%s\"][i]=",$n->{'name'},$n->{'name'};
      if($n->{'type'} eq 'Boolean' or $n->{'type'} eq 'Float' or $n->{'type'} eq 'Integer' or $n->{'type'} eq 'String')
      {
        $rv.=sprintf "Json::Value(e.%s[i]);\n",$n->{'name'};
      }
      else
      {
        $rv.=sprintf "%sMarshaller::toJSON(e.%s[i]);\n",$n->{'type'},$n->{'name'};
      }
    }
    return $rv;
  }

  if(exists $n->{'opt'})
  {
    $rv.=sprintf "  if(e.%s)\n",$n->{'name'};
    $rv.=sprintf "    json[\"%s\"]=",$n->{'name'};
 
    if($n->{'type'} eq 'Boolean' or $n->{'type'} eq 'Float' or $n->{'type'} eq 'Integer' or $n->{'type'} eq 'String')
    {
      $rv.=sprintf "Json::Value(*e.%s);\n",$n->{'name'};
    }
    else
    {
      $rv.=sprintf "%sMarshaller::toJSON(*e.%s);\n",$n->{'type'},$n->{'name'};
    }
  }
  else
  {
    $rv.=sprintf "  json[\"%s\"]=",$n->{'name'};

    if($n->{'type'} eq 'Boolean' or $n->{'type'} eq 'Float' or $n->{'type'} eq 'Integer' or $n->{'type'} eq 'String')
    {
      $rv.=sprintf "Json::Value(e.%s);\n",$n->{'name'};
    }
    else
    {
      $rv.=sprintf "%sMarshaller::toJSON(e.%s);\n",$n->{'type'},$n->{'name'};
    }
  }
  return $rv;
}

sub declare_deserialize
{
    my $n=shift;
    my $rv;
    
    if(exists $n->{'opt'})
    {
      $rv.=sprintf "    if(json.isMember(\"%s\"))\n    {\n",$n->{'name'};

      if(exists $n->{'array'})
      {
        $rv.=sprintf "      const Json::Value& j=json[\"%s\"];\n",$n->{'name'};
        $rv.=sprintf "      if(!j.isArray())  return false;\n";
        $rv.=sprintf "      c.%s=new std::vector<%s>();\n",$n->{'name'},$n->{'ctype'};
        $rv.=sprintf "      c.%s->resize(j.size());\n",$n->{'name'};
        $rv.=sprintf "      for(unsigned int i=0;i<j.size();i++)\n";
      
        if(exists $json{$n->{'type'}})
        {
          $rv.=sprintf "        if(!j[i].is%s())\n",$json2{$n->{'type'}};
          $rv.=sprintf "          return false;\n";
          $rv.=sprintf "        else\n";
          $rv.=sprintf "          c.%s[0][i]=j[i].as%s();\n",$n->{'name'},$json{$n->{'type'}};
        }
        else
        {
          $rv.=sprintf "      {\n";
          $rv.=sprintf "        %s t;\n",$n->{'type'};
          $rv.=sprintf "        if(!%sMarshaller::fromJSON(j[i],t))\n",$n->{'type'};
          $rv.=sprintf "          return false;\n";
          $rv.=sprintf "        c.%s[0][i]=t;\n",$n->{'name'};
          $rv.=sprintf "      }\n\n";
        }
        $rv.="    }\n";
        return $rv;
      }
      else
      {
        $rv.=sprintf "      const Json::Value& j=json[\"%s\"];\n",$n->{'name'};
        if(exists $json{$n->{'type'}})
        {
          $rv.=sprintf "      if(!j.is%s())  return false;\n",$json2{$n->{'type'}};
          $rv.=sprintf "      c.%s=new %s(j.as%s());\n",$n->{'name'},$n->{'ctype'},$json{$n->{'type'}};
        }
        else
        {
          $rv.=sprintf "      c.%s=new %s();\n",$n->{'name'},$n->{'ctype'};
          $rv.=sprintf "      if(!%sMarshaller::fromJSON(j,c.%s[0]))\n",$n->{'type'},$n->{'name'};
          $rv.=sprintf "        return false;\n";
        }
      }
      
      $rv.=sprintf "    }\n",$n->{'name'};
    }
    else
    {
      $rv.=sprintf "    if(!json.isMember(\"%s\"))  return false;\n",$n->{'name'};
      if(exists $n->{'array'})
      {
        $rv.=sprintf "    {\n      const Json::Value& j=json[\"%s\"];\n",$n->{'name'};
        $rv.=sprintf "      if(!j.isArray())  return false;\n";
        $rv.=sprintf "      c.%s.resize(j.size());\n",$n->{'name'};
        $rv.=sprintf "      for(unsigned int i=0;i<j.size();i++)\n";
      
        if(exists $json{$n->{'type'}})
        {
          $rv.=sprintf "        if(!j[i].is%s())\n",$json2{$n->{'type'}};
          $rv.=sprintf "          return false;\n";
          $rv.=sprintf "        else\n";
          $rv.=sprintf "          c.%s[i]=j[i].as%s();\n",$n->{'name'},$json{$n->{'type'}};
        }
        else
        {
          $rv.=sprintf "        {\n";
          $rv.=sprintf "          %s t;\n",$n->{'type'};
          $rv.=sprintf "          if(!%sMarshaller::fromJSON(j[i],t))\n",$n->{'type'};
          $rv.=sprintf "            return false;\n";
          $rv.=sprintf "          c.%s[i]=t;\n",$n->{'name'};
          $rv.=sprintf "        }\n\n";
        }
        $rv.="    }\n";
        return $rv;
      }
      else
      {
        $rv.=sprintf "    {\n      const Json::Value& j=json[\"%s\"];\n",$n->{'name'};
        if(exists $json{$n->{'type'}})
        {
          $rv.=sprintf "      if(!j.is%s())  return false;\n",$json2{$n->{'type'}};
          $rv.=sprintf "      c.%s=j.as%s();\n",$n->{'name'},$json{$n->{'type'}};
        }
        else
        {
          $rv.=sprintf "      if(!%sMarshaller::fromJSON(j,c.%s))\n",$n->{'type'},$n->{'name'};
          $rv.=sprintf "        return false;\n";
        }
        $rv.="    }\n";
      }
    }

  return $rv;
}

foreach(sort keys %types)
{
  my $k=$_;
  next if $types{$k}->{'type'} ne 'struct';
  
  my $cmt=comment($types{$k});

  open(FO,mkpath($path.$k,$sh)) or die;
  print FO header_h($k);
  
  my $t=0+keys %{$types{$k}->{'list'}};
  
  my $fields=prepare_fields($k);
  my @tp=@{$fields->[0]};
  my $isstr=$fields->[1];
  my $isarr=$fields->[2];

  print FO "#include <string>\n" if $isstr==1;
  print FO "#include <vector>\n" if $isarr==1;
  print FO "\n";
  
  foreach(@tp)
  {
    next if $_->{'ref'}==0;
    next if $_->{'ctype'} eq 'std::string';
    printf FO "#include \"%s.%s\"\n",$_->{'ctype'},$sh;
  }

  print FO "\n",$hat;
  print FO "\n";
  printf FO "namespace %s\n{\n",$ns if $ns ne '';
  print FO $cmt;

  my $tmpl=sprintf "\nclass %s\n{\npublic:\n\n  %s(const %s& c);\n  %s(void);\n\n".
  "  bool checkIntegrity(void);\n",$k,$k,$k,$k;


  $tmpl=~s/\n/\n  /gs if $ns ne '';
  printf FO $tmpl;
  
  my $isopt=0;
  foreach(@tp)
  {
    $isopt=1 if exists $_->{'opt'};
  }

  printf FO "\n%s  ~%s(void);\n%s  %s& operator =(const %s&);\n\n",$ind,$k,$ind,$k,$k if $isopt==1;

  print FO "// getters\n\n";

  foreach(@tp)
  {
    my $n=$_;
    if($n->{'ref'}==0 and $n->{'array'}==0)
    {
       printf FO "%s  %s%s%s get_%s(void) const;\n",$ind,(exists $n->{'opt'} ? 'const ' : ''),$n->{'ctype'},(exists $n->{'opt'} ? '*' : ''),$n->{'name'};
    }
    else
    {
      if($n->{'array'}==0)
      {
         printf FO "%s  const %s%s get_%s(void) const;\n",$ind,$n->{'ctype'},(exists $n->{'opt'} ? '*' : '&'),$n->{'name'};
      }
      else
      {
         printf FO "%s  const std::vector<%s>%s get_%s(void) const;\n",$ind,$n->{'ctype'},(exists $n->{'opt'} ? '*' : '&'),$n->{'name'};
      }
    }
  }

  print FO "\n// setters\n\n";
  
  foreach(@tp)
  {
    my $n=$_;
    printf FO "%s  void reset_%s(void);\n",$ind,$n->{'name'} if exists $n->{'opt'};

    printf FO "%s  %s\n",$ind,declare_setter($n);

  }
  
  printf FO "\n%sprivate:\n\n%s  friend class %sMarshaller;\n\n",$ind,$ind,$k;

  foreach(@tp)
  {
#    printf FO "%s  %s\n",$ind,declare_field($_);
    my $n=$_;
    my $c='';
    $c=comment2($n->{'descr'}) if exists $n->{'descr'};
    printf FO "%s%s    %s\n",$c,$ind,declare_field($n);
  }

  printf FO "%s};\n",$ind;
  print FO "\n}\n" if $ns ne '';

  print FO "\n#endif\n";
  close FO;
  
  open(FO,mkpath($pimpl.$k,$sc)) or die;
  print FO $pragma;

  my $nsc= $ns eq '' ? '' : "using namespace $ns;\n\n";
  my $hhat=$hat;
  {
    if(exists $config{'ee'})
    {
      $hhat=~s/^\/\/.*?\n//s;
      $hhat=getfort().$hhat;
    }
  }


  printf FO "#include \"%s/%s.%s\"\n",$p2i,$k,$sh;
  printf FO "#include \"%sMarshaller.%s\"\n",$k,$sh;

  {
    my %h = map { $_->{'ctype'} ,1} grep { $_->{'ref'}==1 and $_->{'ctype'} ne 'std::string' } @tp;
    print FO join("\n",map { sprintf '#include "%sMarshaller.%s"',$_,$sh } sort keys %h);
  }

  print FO $hhat;

  print FO "\n\n";
  print FO $nsc;

  if($isopt==1)
  {
    printf FO "%s& %s::operator =(const %s& c)\n{\n",$k,$k,$k;

    foreach(@tp)
    {
      my $n=$_;
      my $ct=exists $n->{'array'} ? sprintf "std::vector<%s>",$n->{'ctype'} : $n->{'ctype'};
      if(exists $n->{'opt'})
      {
          printf FO "  %s=c.%s ? new %s(c.%s[0]) : 0;\n",$n->{'name'},$n->{'name'},$ct,$n->{'name'};
      }
      else
      {
          printf FO "  %s=c.%s;\n",$n->{'name'},$n->{'name'};
      }
    }
    print FO "\n  return *this;\n";
    printf FO "}\n\n\n%s::~%s(void)\n{\n",$k,$k;
    foreach(@tp)
    {
      my $n=$_;
      next if not exists $n->{'opt'};
      printf FO "  if(%s)\n    delete %s;\n",$n->{'name'},$n->{'name'};
    }
    printf FO "}\n\n\n";
  }
 
  my $tmpl="#name#::#name#(const #name#& c)\n{\n  *this=c;\n}\n\n\nbool #name#::checkIntegrity(void)\n{\n  return #name#Marshaller::checkIntegrity(*this);\n".
  "}\n\n\n#name#::#name#(void)";
  $tmpl=~s/#name#/$k/gs;
  print FO $tmpl;

  if($isopt==1)
  {
    print FO " :\n";
    my @r;
    foreach(@tp)
    {
      next if not exists $_->{'opt'};
      push @r,sprintf "    %s(0)",$_->{'name'};
    }
    print FO join(",\n",@r);
  }

  print FO "\n{\n}\n";

# setters
  print FO "\n\n\n";
  foreach(@tp)
  {
    my $n=$_;
    printf FO "%s",define_setter($n,$k,1);
# reset
    next if not exists $n->{'opt'};
    
    printf FO "void %s::reset_%s(void)\n{\n  if(%s)\n    delete %s;\n  %s=0;\n}\n\n",$k,$n->{'name'},$n->{'name'},$n->{'name'},$n->{'name'};
  }

# getters

  print FO "\n\n\n";
  foreach(@tp)
  {
    my $n=$_;
    if(not exists $n->{'array'} and not exists $n->{'opt'} and $n->{'ref'}==0)
    {
      printf FO "%s %s::get_%s(void) const",$n->{'ctype'},$k,$n->{'name'};
      goto nxt;
    }
    my $z=$n->{'ctype'};
    $z=sprintf "std::vector<%s>",$z if exists $n->{'array'};
    printf FO "const %s%s %s::get_%s(void) const ",$z,(exists $n->{'opt'} ? '*' : '&'),$k,$n->{'name'};
nxt:
    printf FO "\n{\n  return %s;\n}\n\n\n",$n->{'name'};
  }

  close FO;
  
  open(FO,mkpath(sprintf('%s%sMarshaller',$pimpl,$k),$sh)) or die;

  print FO header_h($k.'Marshaller');

  printf FO "#include <string>\n#include <jsoncpp/json.h>\n\n#include \"%s/%s.%s\"\n\n",
        $p2i,$k,$sh;

  print FO $hat;

  printf FO "namespace %s\n{\n",$ns if $ns ne '';

  $tmpl='
struct #name#Marshaller
{
  static bool checkIntegrity(#name#& e);
  static bool checkIntegrityConst(const #name#& e);

  static bool fromString(const std::string& s,#name#& e);
  static const std::string toString(const #name#& e);

  static bool fromJSON(const Json::Value& s,#name#& e);
  static Json::Value toJSON(const #name#& e);
};';

  $tmpl=~s/#name#/$k/gs;
  $tmpl=~s/\n/\n  /gs  if $ns ne '';
  print FO $tmpl;
#  printf FO "%s};\n",$ind;
  print FO "\n}\n" if $ns ne '';

  print FO "\n#endif\n";
  close FO;

  open(FO,mkpath(sprintf('%s%sMarshaller',$pimpl,$k),$sc)) or die;
  print FO $pragma;

  my $nsc= $ns eq '' ? '' : "using namespace $ns;\n\n";
  my $hhat=$hat;
  {
    if(exists $config{'ee'})
    {
      $hhat=~s/^\/\/.*?\n//s;
      $hhat=getfort().$hhat;
    }
  }

  printf FO "#include \"%s/%s.%s\"\n",$p2i,$k,$sh;

  {
    my %h = map { $_->{'ctype'} ,1} grep { $_->{'ref'}==1 and $_->{'ctype'} ne 'std::string' } @tp;
    print FO join("\n",map { sprintf '#include "%sMarshaller.%s"',$_,$sh } sort keys %h);
  }
  
  printf FO "\n\n#include \"%sMarshaller.%s\"\n\n",$k,$sh,$k;

  print FO $hhat;
  
  print FO $nsc;

  $tmpl='
bool #name#Marshaller::checkIntegrity(#name#& s)
{
  return checkIntegrityConst(s);
}


bool #name#Marshaller::fromString(const std::string& s,#name#& e)
{
  try
  {
    Json::Reader reader;
    Json::Value json;
    if(!reader.parse(s,json,false))  return false;
    if(!fromJSON(json,e))  return false;
  }
  catch(...)
  {
    return false;
  }
  return true;
}


const std::string #name#Marshaller::toString(const #name#& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}

';
  $tmpl=~s/#name#/$k/gs;
  print FO $tmpl;

  printf FO "\nbool %sMarshaller::checkIntegrityConst(const %s& s)\n{\n",$k,$k;

  printf FO "%s",declare_check($_) foreach @tp;

  printf FO "  return true;\n}\n";


  printf FO "\nJson::Value %sMarshaller::toJSON(const %s& e)\n{\n  Json::Value json(Json::objectValue);\n  ".
           "if(!checkIntegrityConst(e))\n    return Json::Value(Json::nullValue);\n\n",$k,$k;


  printf FO "%s\n",declare_serialize($_) foreach @tp;

  print FO "\n  return json;\n}\n\n\n";

# from json

  printf FO "bool %sMarshaller::fromJSON(const Json::Value& json,%s& c)\n{\n",$k,$k;
  foreach(@tp)
  {
     next if not exists $_->{'opt'};
     printf FO "  if(c.%s)  delete c.%s;\n  c.%s=0;\n\n",$_->{'name'},$_->{'name'},$_->{'name'};
  }

  printf FO  "  try\n  {\n    if(!json.isObject())  return false;\n\n",$k,$k;

  print FO declare_deserialize($_) foreach @tp;

  printf FO "\n  }\n  catch(...)\n  {\n    return false;\n  }\n".
  "  return checkIntegrity(c);\n}\n\n",$k,$k;

  close FO;

}


# methods. i want sleep, really :-)

my $nsw;
$nsw=$ns.'::' if $ns ne '';

open(FO,'>',sprintf "%sMarshaller.gp",$pimpl) or die;
print FO '%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name Marshaller_intHash
%omit-struct-type
%readonly-tables
%compare-strncmp
%compare-lengths

struct '.$nsw.'Marshaller::localHash
{
  const char *name;
  unsigned int idx;
  void* method;
};

%%
';

for(my $i=0;$i<@table;$i++)
{
  printf FO "%s_%s,%u,&%sMarshaller::m%s%s\n",@table[$i]->[0],@table[$i]->[1],$i,$nsw,@table[$i]->[0],
    (@table[$i]->[1] eq 'notification' ? '' : ('_'.@table[$i]->[1]));
}
close FO;

#my $libs= exists $config{'lib'}  ? $config{'lib'} : '../lib/';


for(my $i=0;$i<@table;$i++)
{
  my $k=@table[$i]->[0];
  $k.='_'.@table[$i]->[1] if @table[$i]->[1] ne 'notification';

  my $ls=@table[$i]->[2];

  my @tp;

  foreach(@$ls)
  {
    my $v=$_;
    my $z=prepare_field($v->{'name'},$v);
    die sprintf "unknown type %s in %s",$v->{'name'},$k if not defined $z;
    $z->{'desc'}=$v->{'description'} if exists $v->{'description'};
    push @tp,$z;
  }

  my %tps;
  my $isstr=0;
  my $isarr=0;
  my $isopt=0;

  foreach(@$ls)
  {
    $tps{$_->{'type'}}=1 if not exists $json{$_->{'type'}};
    $isstr=1 if $_->{'type'} eq 'String';
    $isarr=1 if exists $_->{'array'};
    $isopt=1 if exists $_->{'mandatory'};
  }

  my $mt= 'ALRPC'. ucfirst @table[$i]->[1];

  open(FO,mkpath($path.$k,$sh)) or die;
  print FO header_h($k);

# headers

  printf FO "#include <vector>\n" if $isarr!=0;
  printf FO "#include <string>\n" if $isstr!=0;

  printf FO "\n";
  
  printf FO "#include \"%s.%s\"\n",$_,$sh foreach sort keys %tps;

#  printf FO "#include \"%s%s.hh\"\n\n",$libs,$mt;
  printf FO "#include \"%s/%s.hh\"\n\n",$i2l,$mt;

  print FO $hat;

  printf FO "namespace %s\n{\n",$ns if $ns ne '';
  print FO comment2(@table[$i]->[3]) if defined @table[$i]->[3];

  my $tmpl1='';
  $tmpl1="\n  #name#& operator =(const #name#&);\n" if $isopt==1;

  my $tmpl='
class #name# : public #base#
{
public:

  #name#(const #name#& c);
  #name#(void);
  
  virtual ~#name#(void);
'.$tmpl1.'
  bool checkIntegrity(void);';

  $tmpl=~s/#name#/$k/gs;
  $tmpl=~s/#base#/$mt/gs;
  $tmpl=~s/\n/\n  /gs if $ns ne '';

  print FO $tmpl;
  print FO "\n\n";

  foreach(@tp)
  {
    my $n=$_;
    if($n->{'ref'}==0 and $n->{'array'}==0)
    {
       printf FO "%s  %s%s%s get_%s(void) const;\n",$ind,(exists $n->{'opt'} ? 'const ' : ''),$n->{'ctype'},(exists $n->{'opt'} ? '*' : ''),$n->{'name'};
    }
    else
    {
      if($n->{'array'}==0)
      {
         printf FO "%s  const %s%s get_%s(void) const;\n",$ind,$n->{'ctype'},(exists $n->{'opt'} ? '*' : '&'),$n->{'name'};
      }
      else
      {
         printf FO "%s  const std::vector<%s>%s get_%s(void) const;\n",$ind,$n->{'ctype'},(exists $n->{'opt'} ? '*' : '&'),$n->{'name'};
      }
    }
  }
  printf FO "\n";
 
  foreach(@tp)
  {
    my $n=$_;
    printf FO "%s  void reset_%s(void);\n",$ind,$n->{'name'} if exists $n->{'opt'};
    printf FO "%s  %s\n",$ind,declare_setter($n);
  }
  
  $tmpl=sprintf "\nprivate:\n\n  friend class %sMarshaller;",$k;
  $tmpl=~s/\n/\n  /gs if $ns ne '';
  $tmpl.="\n\n";
  
  print FO $tmpl;

#  printf FO "%s  %s\n",$ind,declare_field($_) foreach @tp;

 foreach(@tp)
 {
   my $n=$_;
   my $c='';
   $c=comment2($n->{'descr'}) if exists $n->{'descr'};
   printf FO "%s%s    %s\n",$c,$ind,declare_field($n);
 } 


  printf FO "%s};\n",$ind;
  print FO "\n}\n" if $ns ne '';
  print FO "\n#endif\n";
  close FO;

  open(FO,mkpath($pimpl.$k,$sc)) or die;
  print FO $pragma;

  my $nsc= $ns eq '' ? '' : "using namespace $ns;\n";
  my $hhat=$hat;

  if(exists $config{'ee'})
  {
     $hhat=~s/^\/\/.*?\n//s;
     $hhat=getfort().$hhat;
  }

  printf FO  "#include \"%s/%s.%s\"\n#include \"%sMarshaller.%s\"\n",$p2i,$k,$sh,$k,$sh;
  printf FO  "#include \"%s/Marshaller.%s\"\n",$p2i,$sh;
  printf FO  "#include \"%sMarshaller.%s\"\n",$_,$sh foreach sort keys %tps;

  printf FO "\n#define PROTOCOL_VERSION\t%u\n\n",$pver;

  print FO $hhat;
  print FO $nsc if $ns ne '';
  
  if($isopt==1)
  {
    printf FO "%s& %s::operator =(const %s& c)\n{\n",$k,$k,$k;

    foreach(@tp)
    {
      my $n=$_;
      my $ct=exists $n->{'array'} ? sprintf "std::vector<%s>",$n->{'ctype'} : $n->{'ctype'};
      if(exists $n->{'opt'})
      {
          printf FO "  %s= c.%s ? new %s(c.%s[0]) : 0;\n",$n->{'name'},$n->{'name'},$ct,$n->{'name'};
      }
      else
      {
          printf FO "  %s= c.%s;\n",$n->{'name'},$n->{'name'};
      }
    }
    print FO "\n  return *this;}\n\n"
  }

  printf FO "\n%s::~%s(void)\n{\n",$k,$k;
  foreach(@tp)
  {
    my $n=$_;
    next if not exists $n->{'opt'};
    printf FO "  if(%s)\n    delete %s;\n",$n->{'name'},$n->{'name'};
  }
  
  printf FO "}\n\n\n"; 
  my $tmpl="#name#::#name#(const #name#& c)\n{\n  *this=c;\n}\n\n\nbool #name#::checkIntegrity(void)\n{\n  return #name#Marshaller::checkIntegrity(*this);\n".
  "}\n\n\n#name#::#name#(void) : #base#(PROTOCOL_VERSION,Marshaller::METHOD_#NAME#)";
  $tmpl=~s/#name#/$k/gs;
  my $z=uc $k;
  $tmpl=~s/#NAME#/$z/gs;
  $tmpl=~s/#base#/$mt/gs;

#  $z=uc @table[$i]->[1];
#  $tmpl=~s/#func#/$z/gs;
  print FO $tmpl;

  if($isopt==1)
  {
    print FO ",\n  ";
    my @r;
    foreach(@tp)
    {
      next if not exists $_->{'opt'};
      push @r,sprintf "    %s(0)",$_->{'name'};
    }
    print FO join(",\n",@r);
  }

  print FO "\n{\n}\n";

# setters
  print FO "\n\n\n";
  foreach(@tp)
  {
    my $n=$_;
    printf FO "%s",define_setter($n,$k,1);
# reset
    next if not exists $n->{'opt'};
    
    printf FO "void %s::reset_%s(void)\n{\n  if(%s)\n    delete %s;\n  %s=0;\n}\n\n",$k,$n->{'name'},$n->{'name'},$n->{'name'},$n->{'name'};
  }

# getters

  print FO "\n\n\n";
  foreach(@tp)
  {
    my $n=$_;
    if(not exists $n->{'array'} and not exists $n->{'opt'} and $n->{'ref'}==0)
    {
      printf FO "%s %s::get_%s(void) const",$n->{'ctype'},$k,$n->{'name'};
      goto nxt;
    }
    my $z=$n->{'ctype'};
    $z=sprintf "std::vector<%s>",$z if exists $n->{'array'};
    printf FO "const %s%s %s::get_%s(void) const ",$z,(exists $n->{'opt'} ? '*' : '&'),$k,$n->{'name'};
nxt:
    printf FO "\n{\n  return %s;\n}\n\n",$n->{'name'};
  }

  close FO;

  open(FO,mkpath(sprintf('%s%sMarshaller',$pimpl,$k),$sh)) or die;

  print FO header_h($k.'Marshaller');

  printf FO "#include <string>\n#include <jsoncpp/json.h>\n\n#include \"%s/%s.%s\"\n\n",
        $p2i,$k,$sh;

  print FO $hat;

  printf FO "namespace %s\n{\n",$ns if $ns ne '';

  $tmpl='
struct #name#Marshaller
{
  static bool checkIntegrity(#name#& e);
  static bool checkIntegrityConst(const #name#& e);

  static bool fromString(const std::string& s,#name#& e);
  static const std::string toString(const #name#& e);

  static bool fromJSON(const Json::Value& s,#name#& e);
  static Json::Value toJSON(const #name#& e);
};';

  $tmpl=~s/#name#/$k/gs;
  $tmpl=~s/\n/\n  /gs  if $ns ne '';
  print FO $tmpl;
#  printf FO "%s};\n",$ind;
  print FO "\n}\n" if $ns ne '';

  print FO "\n#endif\n";
  close FO;

  open(FO,mkpath(sprintf('%s%sMarshaller',$pimpl,$k),$sc)) or die;
  print FO $pragma;

  my $nsc= $ns eq '' ? '' : "using namespace $ns;\n\n";
  my $hhat=$hat;
  {
    if(exists $config{'ee'})
    {
      $hhat=~s/^\/\/.*?\n//s;
      $hhat=getfort().$hhat;
    }
  }

  printf FO "#include \"%s/%s.%s\"\n",$p2i,$k,$sh;

  {
    my %h = map { $_->{'ctype'} ,1} grep { $_->{'ref'}==1 and $_->{'ctype'} ne 'std::string' } @tp;
    print FO join("\n",map { sprintf '#include "%sMarshaller.%s"',$_,$sh } sort keys %h);
  }
  
  printf FO "\n\n#include \"%sMarshaller.%s\"\n\n",$k,$sh,$k;

  print FO $hhat;
  
  print FO $nsc;

  $tmpl='
bool #name#Marshaller::checkIntegrity(#name#& s)
{
  return checkIntegrityConst(s);
}


bool #name#Marshaller::fromString(const std::string& s,#name#& e)
{
  try
  {
    Json::Reader reader;
    Json::Value json;
    if(!reader.parse(s,json,false))  return false;
    if(!fromJSON(json,e))  return false;
  }
  catch(...)
  {
    return false;
  }
  return true;
}


const std::string #name#Marshaller::toString(const #name#& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}

';
  $tmpl=~s/#name#/$k/gs;
  print FO $tmpl;

  printf FO "\nbool %sMarshaller::checkIntegrityConst(const %s& s)\n{\n",$k,$k;

  printf FO "%s",declare_check($_) foreach @tp;

  printf FO "  return true;\n}\n";


  printf FO "\nJson::Value %sMarshaller::toJSON(const %s& e)\n{\n  Json::Value json(Json::objectValue);\n  ".
           "if(!checkIntegrityConst(e))\n    return Json::Value(Json::nullValue);\n\n",$k,$k;


  $tmpl='  json["#meth#"]=Json::Value(Json::objectValue);
  json["#meth#"]["name"]=Json::Value("#name#");
'. (@table[$i]->[1] eq 'notification' ? '' :  '  json["#meth#"]["correlationID"]=Json::Value(e.getCorrelationID());
').'
  Json::Value j=Json::Value(Json::objectValue);

';

  $z=@table[$i]->[1];
  $tmpl=~s/#meth#/$z/gs;
  $z=@table[$i]->[0];
  $tmpl=~s/#name#/$z/gs;

  print FO $tmpl;

  foreach(@tp)
  {
    $z=declare_serialize($_);
    $z=~s/json\[/j[/gs;
    printf FO "%s\n",$z;
  }
  printf FO '  json["%s"]["parameters"]=j;',@table[$i]->[1];

  print FO "\n  return json;\n}\n\n\n";

  printf FO "bool %sMarshaller::fromJSON(const Json::Value& js,%s& c)\n{\n",$k,$k;
  foreach(@tp)
  {
     next if not exists $_->{'opt'};
     printf FO "  if(c.%s)  delete c.%s;\n  c.%s=0;\n\n",$_->{'name'},$_->{'name'},$_->{'name'};
  }

  printf FO  "  try\n  {\n    if(!js.isObject())  return false;\n\n",$k,$k;

  my $tmpl2;
  $tmpl2='    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());
' if @table[$i]->[1] ne 'notification';

  $tmpl='    if(!js.isMember("#type#"))  return false;

    if(!js["#type#"].isObject())  return false;
    const Json::Value& j2=js["#type#"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("#name#"))  return false;
'.$tmpl2.'
    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
';

  $z=@table[$i]->[1];
  $tmpl=~s/#type#/$z/gs;
  $z=@table[$i]->[0];
  $tmpl=~s/#name#/$z/gs;
  print FO $tmpl;
  print FO declare_deserialize($_) foreach @tp;

  printf FO "\n  }\n  catch(...)\n  {\n    return false;\n  }\n".
  "  return checkIntegrity(c);\n}\n\n",$k,$k;

  close FO;
}

# Marshaller cc/hh
# last touch

sub mkfuncname
{
  my $t=shift;
  return $_->[1] eq 'notification' ? $_->[0] : sprintf "%s_%s",$_->[0],$_->[1];
}

open(FO,mkpath($path.'Marshaller',$sh)) or die;
print FO header_h('Marshaller');
print FO "#include <string>\n#include <jsoncpp/json.h>\n\n";

foreach(@table)
{
  my $z=$_->[1] eq 'notification' ? $_->[0] : sprintf "%s_%s",$_->[0],$_->[1];
  printf FO "#include \"%s/%sMarshaller.%s\"\n",$i2p,$z,$sh;
}

print FO "\n";
print FO $hat;

printf FO "namespace %s\n{\n",$ns if $ns ne '';

my $tmpl="#class Marshaller\n#{\n#public:\n\n#  enum Methods\n#  {\n#    METHOD_INVALID=-1";

$tmpl=~s/#/$ind/gs;

print FO $tmpl;

my $z;

my @lc=map { uc mkfuncname $_ } @table;
for(my $i=0;$i<@lc;$i++)
{
  printf FO ",\n    %sMETHOD_%s=%u",$ind,@lc[$i],$i;
}
printf FO "\n  %s};\n",$ind;


$tmpl='
  Marshaller()					{}
  ~Marshaller()					{}
  

  static ALRPCMessage* fromString(const std::string&);
  static std::string toString(const ALRPCMessage* msg);

protected:
  static ALRPCMessage* fromJSON(const Json::Value&);
  static Json::Value toJSON(const ALRPCMessage* msg);';
  
$tmpl=~s/\n/\n  /gs if $ns ne '';

print FO $tmpl;
print FO "\n";

foreach(@table)
{
  my $k=$_;
  my $n=mkfuncname($k);
  printf FO "\n// %s\n",$n;
  $tmpl="#  static bool fromString(const std::string& str, #name#& res)\n#  {\n#    return m#name#.fromString(str, res);\n#  }\n\n#  ".
        "static std::string toString(const #name#& res)\n#  {\n#    return m#name#.toString(res);\n#  }\n";
  $tmpl=~s/#name#/$n/gs;
  $tmpl=~s/#/$ind/gs;
  print FO $tmpl;
}

$tmpl='
  struct localHash
  {
    const char *name;
    unsigned int idx;
    void* marshaller;					// Bingo!!!  old good plain C with direct casting!!!
  };

private:';

$tmpl=~s/\n/\n  /gs if $ns ne '';

print FO $tmpl;
print FO "\n\n";

printf FO "%s  static %sMarshaller m%s;\n",$ind,mkfuncname($_),mkfuncname($_) foreach @table;

$tmpl=sprintf '
  static const char* getName(Methods e)
  {
     return (e>=0 && e<%u) ? mHashTable[e].name : NULL;
  }
   
  static const Methods getIndex(const char* s);

  static const localHash mHashTable[%u];
  
  friend class Marshaller_intHash;',scalar @table,scalar @table; #,($ns eq '' ? '' : $ns.'::');

$tmpl=~s/\n/\n  /gs if $ns ne '';
print FO $tmpl;

printf FO "\n%s};\n",$ind;
print FO "\n}\n" if $ns ne '';

print FO "\n#endif\n";
close FO;


open(FO,mkpath($pimpl.'Marshaller',$sc)) or die;
print FO $pragma;

my $nsc= $ns eq '' ? '' : "using namespace $ns;\n\n";
my $hhat=$hat;
{
  if(exists $config{'ee'})
  {
    $hhat=~s/^\/\/.*?\n//s;
    $hhat=getfort().$hhat;
  }
}

printf FO "#include <cstring>\n\n#include \"%s/Marshaller.%s\"\n",$p2i,$sh;
printf FO "#include \"%s/%s.%s\"\n",$p2i,mkfuncname($_),$sh foreach @table;

printf FO "namespace %s\n{",$ns if $ns ne '';
print FO "\n#include \"Marshaller.inc\"\n";
print FO "}\n" if $ns ne '';

print FO "\n";

print FO $hhat;

print FO $nsc;

print FO '
const Marshaller::Methods Marshaller::getIndex(const char* s)
{
  if(!s)
    return METHOD_INVALID;
  const struct localHash* p=Marshaller_intHash::getPointer(s,strlen(s));
  return p ? static_cast<Methods>(p->idx) : METHOD_INVALID;
}


ALRPCMessage* Marshaller::fromString(const std::string& s)
{
  ALRPCMessage* rv=0;
  try
  {
    Json::Reader reader;
    Json::Value json;

    if(!reader.parse(s,json,false))  return 0;
    if(!(rv=fromJSON(json)))  return 0;
  }
  catch(...)
  {
    return 0;
  }
  return rv;
}

std::string Marshaller::toString(const ALRPCMessage* msg)
{
  if(!msg)  return "";

  Json::Value json=toJSON(msg);

  if(json.isNull()) return "";

  Json::FastWriter writer;
  std::string rv;
  return writer.write(json);
}


ALRPCMessage* Marshaller::fromJSON(const Json::Value& json)
{
  if(!json.isObject())  return NULL;
  Json::Value j=Json::Value(Json::nullValue);

  std::string tp;
  if(json.isMember("notification")) j=json[tp="notification"];

  if(json.isMember("request"))  j=json[tp="request"];
  if(json.isMember("response"))  j=json[tp="response"];

  if(!j.isObject()) return NULL;
  if(!j.isMember("name") || !j["name"].isString()) return NULL;

  std::string ts=j["name"].asString()+"_"+tp;

  Methods m=getIndex(ts.c_str());
  switch(m)							// let'."'".'s rock, china style
  {
    case METHOD_INVALID:
      return NULL;
';

foreach(@table)
{
  my $n=$_;
  my $t='
    case METHOD_#NAME#:
    {
      #name#* rv=new #name#;
      if(#name#Marshaller::fromJSON(json,*rv))
        return rv;
      delete rv;
      return NULL;
    }
';
  my $z=mkfuncname($n);
  $t=~s/#name#/$z/gs;
  $z=uc $z;
  $t=~s/#NAME#/$z/gs;
  print FO $t;
}
print FO "  }\n\n  return NULL;\n}\n\n";

print FO 'Json::Value Marshaller::toJSON(const ALRPCMessage* msg)
{
  Json::Value j=Json::Value(Json::nullValue);

  if(!msg) return j;
  Methods m=static_cast<Methods>(msg->getMethodId());

  switch(m)
  {';

foreach(@table)
{
  my $n=$_;
  my $t='
    case METHOD_#NAME#:
         return #name#Marshaller::toJSON(* static_cast<const #name#*>(msg));
';
  my $z=mkfuncname($n);
  $t=~s/#name#/$z/gs;
  $z=uc $z;
  $t=~s/#NAME#/$z/gs;
  print FO $t;
}

print FO "\n    case METHOD_INVALID:\n    default:\n      return j;\n  }\n\n  return j;\n}\n\n";


printf FO "%sMarshaller Marshaller::m%s;\n",mkfuncname($_),mkfuncname($_) foreach @table;

printf FO "\nconst Marshaller::localHash Marshaller::mHashTable[%u]=\n{\n",scalar @table;

my @lc;
@lc= map { sprintf '  {"%s_%s",METHOD_%s,&Marshaller::m%s}',$_->[0],$_->[1],uc mkfuncname($_),mkfuncname($_) } @table;


print FO join(",\n",@lc);

print FO "\n};\n";

close FO;


open(FO,mkpath($path.'RPC',$sh)) or die;
print FO header_h('RPC');
print FO "#include <string>\n#include <jsoncpp/json.h>\n\n";

printf FO "#include \"%s.%s\"\n",$_,$sh foreach sort keys %types;

printf FO "#include \"%s.%s\"\n",mkfuncname($_),$sh foreach @table;


print FO "\n";
print FO $hat;

print FO "\n\n#endif\n";

print STDERR "done\n";
#die Dumper \%types;
exit 0;

