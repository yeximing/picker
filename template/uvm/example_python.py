# File       : example_uvm.sv
# Author     : automatically generated by picker
# Date       : {{datenow}}
# Description: This is an example of using the picker with Python. 
#              In this example, python sub the msg from UVM and parse it to object,then
#              pub the object to UVM
# Version    : {{version}}

import tlm_pbsb as u
import sys
sys.path.append('../')
from {{className}}_xagent import *

def subscribe_message(message,publish):
    transaction_list= {{className}}_list(message).transaction_list
    # convert msg to pthon class
    for transaction in transaction_list:
        print("python sub tr", vars(transaction))
        publish_message(transaction,publish)


def publish_message({{className}},publish):
    uvm_message = u.tlm_msg()
    byte_stream = {{className}}.to_msg()
    print("python sub tr ",byte_stream)
    uvm_message .from_bytes(byte_stream)
    publish.SendMsg(uvm_message)
    

def test_msg():
    # build publish and subscribe channel
    publish = u.TLMPub("{{className}}")
    subscribe = u.TLMSub("{{className}}", lambda a: subscribe_message(a.as_bytes(),publish))

    # connect channel
    publish.Connect()
    subscribe.Connect()

    # initial vcs
    u.tlm_vcs_init("_tlm_pbsb.so", "-no_save")

    for i in range(500):
        # step uvm + systemc
        u.step(1)


if __name__ == "__main__":
    test_msg()
