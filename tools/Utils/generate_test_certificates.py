#!/usr/bin/env python2
# -*- coding: utf-8 -*-

"""Generate certificates for testing
  Usage:
    generate_test_certificate.py --dir=<name of directory>
"""

import os
import subprocess
import tempfile
from argparse import ArgumentParser
from subprocess import check_call

def run(command, *args):
    """Application caller
    wrap console call 'command args'
    """
    args_str = " " + " ".join(str(s) for s in list(args[0]))
    if len(args_str) > 0 :
        command += " " + args_str
    print "Running", command
    retcode = check_call(command, shell=True)
    if retcode < 0:
        raise RuntimeError("Child was terminated by signal")

def openssl(*args):
    """OpenSSL caller
    wrap console call 'openssl args'
    """
    run("openssl", args)

def gen_rsa_key(out_key_file, key_size):
    """Private key generator
    wrap console call 'openssl genrsa -out $out_key_file $key_size'
    """
    openssl("genrsa", "-out", out_key_file, key_size)

def gen_root_cert(out_cert_file, key_file, days, answer):
    """Root certificate generator
    wrap console call 'openssl req -x509 -new -key $key_file -days $days -out $out_cert_file -subj $answer'
    """
    openssl("req -x509 -new -key", key_file, "-days", days, "-out", out_cert_file, "-subj", answer)

def gen_cert(out_cert_file, key_file, ca_cert_file, ca_key_file, days, answer):
    """Certificate generator
    wrap console call
    'openssl req -new -key $key_file -days $days -out $out_cert_file -subj $answer'
    'openssl x509 -req -in $out_cert_file -CA $ca_cert_file -CAkey ca_key_file -CAcreateserial -out $out_cert_file -days 5000'
    """
    request_file = out_cert_file + ".req"
    openssl("req -new -key", key_file, "-days", days, "-out", request_file, "-subj", answer)

    openssl("x509 -hash -req -in", request_file, "-CA", ca_cert_file, "-CAkey", ca_key_file, \
        "-CAcreateserial -out", out_cert_file, "-days", days)

def gen_expire_cert(out_cert_file, key_file, ca_cert_file, ca_key_file, days, answer):
    """Expired certificate generator
    wrap console call
    'openssl req -new -key $key_file -days $days -out $out_cert_file -subj $answer'
    'openssl ca -batch -config $config_file_path -in $request_file -out $out_cert_file, 
        "-cert",  ca_cert_file, "-keyfile", ca_key_file, "-startdate 150101000000Z -enddate 150314092653Z'
    """
    request_file = out_cert_file + ".req"
    openssl("req -new -key", key_file, "-days", days, "-out", request_file, "-subj", answer)

    # Create temporary files needed for expired certificate generation
    temp_dir = tempfile.mkdtemp()
    config_file_path = os.path.join(temp_dir, "database.conf")
    database_file_path = os.path.join(temp_dir, "database")
    serial_file_path = os.path.join(temp_dir, "serial")
    # create file
    open(database_file_path, 'w').close()

    serial_file = open(serial_file_path, 'w')
    serial_file.write("01")
    serial_file.close();

    current_dir = os.getcwd()
    config_file = open(config_file_path, 'w')
    config_file.write(
        """[ ca ]
        default_ca = ca_default

        [ ca_default ]
        dir = %s""" % (temp_dir, ) + """ 
        certs = %s""" % (current_dir, ) + """
        new_certs_dir =  %s""" % (current_dir, ) + """
        database = %s""" % (database_file_path, ) + """
        serial = %s""" % (serial_file_path, ) + """
        RANDFILE = $dir/ca.db.rand
        certificate =  %s""" % (os.path.abspath(ca_cert_file), ) + """
        private_key =  %s""" % (os.path.abspath(ca_key_file), ) + """
        default_days = 365
        default_crl_days = 30
        default_md = md5
        preserve = no
        policy = generic_policy
        [ generic_policy ]
        countryName = optional
        stateOrProvinceName = optional
        localityName = optional
        organizationName = optional
        organizationalUnitName = optional
        commonName = supplied
        emailAddress = optional\n""")
    config_file.close();

    openssl("ca -batch -config", config_file_path, "-in", request_file, "-out", out_cert_file,
        "-startdate 150101000000Z -enddate 150314092653Z")

def gen_pkcs12(out, key_file, cert_file, verification_certificate) :
    """Pem to PKCS#12 standard
    wrap console call
    'openssl pkcs12 -export -out $out -inkey $key_file -in $cert_file -name 'SPT key and certificates' -certfile $certs'
    """
    openssl("pkcs12 -export -out", out, "-inkey", key_file, "-in", cert_file, \
       "-name 'SPT key and certificates'", "-CAfile ", verification_certificate, \
       " -passout pass:")

    """
    Encode certificate $out to base 64
    """
    with open(out, "rb") as cert:
        with open(out + ".enc", "wb") as enc_cert:
            enc_cert.write(cert.read().encode("base64"))

def answers(name, app_id, country, state, locality, organization, unit, email) :
    """Answer string generator
    Generate answer for certificate creation with openssl
    Country argument need to be 2 symbol size
    """
    if len(country) != 2 :
        raise ValueError("Country argument need to be 2 symbol size")
    answer ="'/C={0}/ST={1}/L={2}/O={3}".format(country, state, locality, organization)
    answer +="/OU={0}/CN={1}/emailAddress={2}'".format(unit, name, email)
    if len(app_id) > 0:
        answer += "/serialNumber={0}".format(app_id)
    return answer

def concat_files(out_file_name, *args) :
    print "Concatenate text files", args, "into", out_file_name 
    with open(out_file_name, 'w') as outfile:
        for fname in args :
            with open(fname) as infile :
                outfile.write(infile.read())


def main():
    soft_verify_file  = os.path.join("server", "server_root.key")
    arg_parser = ArgumentParser(description='Welcome to SDL test certificate generator.')
    arg_parser.add_argument('-d', '--dir', help="directory for certificate generating")
    arg_parser.add_argument('-s', '--soft', help="do not override existing certificates if '%s' exists" % soft_verify_file, action='store_true' )
    args = arg_parser.parse_args()
    if args.dir:
        if not os.path.exists(args.dir):
            raise OSError("Input directory does not exist")
        os.chdir(args.dir)
    if args.soft:
        if os.path.exists(soft_verify_file):
            print "Root key file '%s' exists. Generation skipped according to soft mode." % (soft_verify_file, )
            return

    server_root_answer = answers("server_root", "", "US", "California", "Silicon Valley", "CAcert.org", "CAcert", "sample@cacert.org")
    client_root_answer = answers("client_root", "", "US", "California", "Silicon Valley", "CAcert.org", "CAcert", "sample@cacert.org")
    ford_server_answer = answers("FORD", "", "US", "Michigan", "Detroit", "FORD_SERVER", "FORD_SDL_SERVER" ,"sample@ford.com")
    ford_client_answer = answers("FORD_CLIENT", "", "US", "Michigan", "Detroit", "FORD_CLIENT", "FORD_SDL_CLIENT" ,"sample@ford.com")
    client_answer  = answers("client", "SPT", "RU", "Russia", "St. Petersburg", "Luxoft", "HeadUnit" ,"sample@luxoft.com")
    server_answer  = answers("server", "SPT", "RU", "Russia", "St. Petersburg", "Luxoft", "Mobile" ,"sample@luxoft.com")
    server_unsigned_answer  = answers("server", "SPT", "RU", "Russia", "St. Petersburg", "Luxoft", "Mobile_unsigned" ,"sample@luxoft.com")
    server_expired_answer  = answers("server", "SPT", "RU", "Russia", "St. Petersburg", "Luxoft", "Mobile_expired" ,"sample@luxoft.com")
    client_unsigned_answer  = answers("client", "SPT", "RU", "Russia", "St. Petersburg", "Luxoft", "Mobile_unsigned" ,"sample@luxoft.com")
    client_expired_answer  = answers("client", "SPT", "RU", "Russia", "St. Petersburg", "Luxoft", "Mobile_expired" ,"sample@luxoft.com")
    days = 10000

    server_dir = "server"
    client_dir = "client"
    if not os.path.exists(server_dir):
        os.mkdir(server_dir)
    if not os.path.exists(client_dir):
        os.mkdir(client_dir)

    print " --== Root certificate generating SERVER==-- "
    server_root_key_file  = os.path.join(server_dir, "server_root.key")
    assert soft_verify_file == server_root_key_file, 'Update soft key file path'
    server_root_cert_file = os.path.join(server_dir, "server_root.crt")
    gen_rsa_key(server_root_key_file, 2048)
    gen_root_cert(server_root_cert_file, server_root_key_file, days, server_root_answer)

    print " --== Root certificate generating CLIENT==-- "
    client_root_key_file  = os.path.join(client_dir, "client_root.key")
    client_root_cert_file = os.path.join(client_dir, "client_root.crt")
    gen_rsa_key(client_root_key_file, 2048)
    gen_root_cert(client_root_cert_file, client_root_key_file, days, client_root_answer)

    print
    print " --== Ford server CA certificate generating ==-- "
    ford_server_key_file  = os.path.join(server_dir, "ford_server.key")
    ford_server_cert_file = os.path.join(server_dir, "ford_server.crt")
    gen_rsa_key(ford_server_key_file, 2048)
    gen_cert(ford_server_cert_file, ford_server_key_file, server_root_cert_file, server_root_key_file, days, ford_server_answer)

    print
    print " --== Ford client CA certificate generating ==-- "
    ford_client_key_file  = os.path.join(client_dir, "ford_client.key")
    ford_client_cert_file = os.path.join(client_dir, "ford_client.crt")
    gen_rsa_key(ford_client_key_file, 2048)
    gen_cert(ford_client_cert_file, ford_client_key_file, client_root_cert_file, client_root_key_file, days, ford_client_answer)

    print
    print " --== SDL and SPT adjustment  ==-- "
    server_verification_ca_cert_file = os.path.join(server_dir, "server_verification_ca_cetrificates.crt")
    client_verification_ca_cert_file = os.path.join(client_dir,  "client_verification_ca_cetrificates.crt")
    concat_files(server_verification_ca_cert_file, server_root_cert_file, ford_server_cert_file)
    concat_files(client_verification_ca_cert_file, client_root_cert_file, ford_client_cert_file)


    print
    print " --== Client certificate generating ==-- "
    client_key_file  = os.path.join(client_dir, "client.key")
    client_cert_file = os.path.join(client_dir, "client.crt")
    gen_rsa_key(client_key_file, 2048)
    gen_cert(client_cert_file, client_key_file, ford_client_cert_file, ford_client_key_file, days, client_answer)


    print
    print " --== Server certificate generating ==-- "
    server_key_file = os.path.join(server_dir, "server.key")
    server_cert_file = os.path.join(server_dir, "server.crt")
    server_pkcs12_file = os.path.join(server_dir, "spt_credential.p12")
    gen_rsa_key(server_key_file, 2048)
    gen_cert(server_cert_file, server_key_file, ford_server_cert_file, ford_server_key_file, days, server_answer)
    gen_pkcs12(server_pkcs12_file, server_key_file, server_cert_file, client_verification_ca_cert_file)

    print
    print " --== Server unsigned certificate generating ==-- "
    server_unsigned_cert_file = os.path.join(server_dir, "server_unsigned.crt")
    server_pkcs12_unsigned_file = os.path.join(server_dir, "spt_credential_unsigned.p12")
    gen_root_cert(server_unsigned_cert_file, server_key_file, days, server_unsigned_answer)
    gen_pkcs12(server_pkcs12_unsigned_file, server_key_file, server_unsigned_cert_file, client_verification_ca_cert_file)

    print
    print " --== Server expired certificate generating ==-- "
    server_expired_cert_file = os.path.join(server_dir, "server_expired.crt")
    server_pkcs12_expired_file = os.path.join(server_dir, "spt_credential_expired.p12")
    gen_expire_cert(server_expired_cert_file, server_key_file, ford_server_cert_file, ford_server_key_file, days, server_expired_answer)
    gen_pkcs12(server_pkcs12_expired_file, server_key_file, server_expired_cert_file, client_verification_ca_cert_file)


    print
    print " --== Client pkcs12 certificate generating ==-- "
    client_key_file = os.path.join(client_dir, "client.key")
    client_cert_file = os.path.join(client_dir, "client.crt")
    client_pkcs12_file = os.path.join(client_dir, "client_credential.p12")
    gen_rsa_key(client_key_file, 2048)
    gen_cert(client_cert_file, client_key_file, ford_client_cert_file, ford_client_key_file, days, client_answer)
    gen_pkcs12(client_pkcs12_file, client_key_file, client_cert_file, server_verification_ca_cert_file)

    print
    print " --== Client pkcs12 unsigned certificate generating ==-- "
    client_unsigned_cert_file = os.path.join(client_dir, "client_unsigned.crt")
    client_pkcs12_unsigned_file = os.path.join(client_dir, "client_credential_unsigned.p12")
    gen_root_cert(client_unsigned_cert_file, client_key_file, days, client_unsigned_answer)
    gen_pkcs12(client_pkcs12_unsigned_file, client_key_file, client_unsigned_cert_file, server_verification_ca_cert_file)

    print
    print " --== Client pkcs12 expired certificate generating ==-- "
    client_expired_cert_file = os.path.join(client_dir, "client_expired.crt")
    client_pkcs12_expired_file = os.path.join(client_dir, "client_credential_expired.p12")
    gen_expire_cert(client_expired_cert_file, client_key_file, ford_client_cert_file, ford_client_key_file, days, client_expired_answer)
    gen_pkcs12(client_pkcs12_expired_file, client_key_file, client_expired_cert_file, server_verification_ca_cert_file)

    subprocess.call(["c_rehash", server_dir])
    subprocess.call(["c_rehash", client_dir])
    print
    print "All certificates have been generated"

if __name__ == "__main__":
    main()
