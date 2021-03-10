#!/usr/bin/env python3

# Test whether an unsupported extended auth is rejected.

from mosq_test_helper import *

def do_test(start_broker):
    rc = 1

    props = mqtt5_props.gen_string_prop(mqtt5_props.PROP_AUTHENTICATION_METHOD, "unsupported")
    props += mqtt5_props.gen_string_prop(mqtt5_props.PROP_AUTHENTICATION_DATA, "test")
    connect_packet = mosq_test.gen_connect("client-params-test", keepalive=42, proto_ver=5, properties=props)
    connack_packet = mosq_test.gen_connack(rc=mqtt5_rc.MQTT_RC_BAD_AUTHENTICATION_METHOD, proto_ver=5, properties=None)

    port = mosq_test.get_port()
    if start_broker:
        broker = mosq_test.start_broker(filename=os.path.basename(__file__), port=port)

    try:
        sock = mosq_test.do_client_connect(connect_packet, connack_packet, timeout=20, port=port)
        rc = 0

        sock.close()
    except mosq_test.TestError:
        pass
    finally:
        if start_broker:
            broker.terminate()
            broker.wait()
            (stdo, stde) = broker.communicate()
            if rc:
                print(stde.decode('utf-8'))
                exit(rc)
        else:
            return rc


def all_tests(start_broker=False):
    return do_test(start_broker)

if __name__ == '__main__':
    all_tests(True)
