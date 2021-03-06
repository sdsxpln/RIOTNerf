# name of your application
APPLICATION = coap_test

# If no BOARD is found in the environment, use this default:
BOARD ?= native

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= ../../../../../RIOT

BOARD_INSUFFICIENT_MEMORY := airfy-beacon chronos msb-430 msb-430h nrf51dongle \
                          nrf6310 pca10000 pca10005 spark-core \
                          stm32f0discovery telosb weio wsn430-v1_3b wsn430-v1_4 \
                          yunjia-nrf51822 z1 nucleo-f072 nucleo-f030 nucleo-f070

# Include packages that pull up and auto-init the link layer.
# NOTE: 6LoWPAN will be included if IEEE802.15.4 devices are present
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
# Specify the mandatory networking modules for IPv6 and UDP
USEMODULE += gnrc_ipv6_router_default
USEMODULE += gnrc_udp
# Add a routing protocol
USEMODULE += gnrc_rpl
# Additional networking modules that can be dropped if not needed
USEMODULE += gnrc_icmpv6_echo

#
USEMODULE += gnrc_conn_udp

USEMODULE += shell

USEMODULE += ps

USEMODULE += xtimer

USEMODULE += cpp11-compat

USEPKG += libcoap

# include this for printing IP addresses
USEMODULE += shell_commands

USEMODULE += gnrc_ipv6_default

USEMODULE += gnrc_conn_udp

USEMODULE += posix_sockets

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
CFLAGS += -DDEVELHELP

# Use different settings when compiling for one of the following (low-memory)
# boards
LOW_MEMORY_BOARDS := nucleo-f334

ifneq (,$(filter $(BOARD),$(LOW_MEMORY_BOARDS)))
$(info Using low-memory configuration for microcoap_server.)
## low-memory tuning values
# lower pktbuf buffer size
CFLAGS += -DGNRC_PKTBUF_SIZE=1000
# disable fib, rpl
DISABLE_MODULE += fib gnrc_rpl
USEMODULE += prng_minstd
endif

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

FEATURES_REQUIRED += cpp

CXXEXFLAGS += -std=gnu++1y

include $(RIOTBASE)/Makefile.include

CXXEXFLAGS += -fpermissive
CXXEXFLAGS += -Wno-error

# Set a custom channel if needed
ifeq (,$(filter cc110x,$(USEMODULE)))     # radio is cc110x sub-GHz
  DEFAULT_CHANNEL ?= 0
  CFLAGS += -DCC110X_DEFAULT_CHANNEL=$(DEFAULT_CHANNEL)
else
  ifeq (,$(filter at86rf212b,$(USEMODULE)))   # radio is IEEE 802.15.4 sub-GHz
    DEFAULT_CHANNEL ?= 5
    CFLAGS += -DIEEE802154_DEFAULT_SUBGHZ_CHANNEL=$(DEFAULT_CHANNEL)
  else                                      # radio is IEEE 802.15.4 2.4 GHz
    DEFAULT_CHANNEL ?= 26
    CFLAGS += -DIEEE802154_DEFAULT_CHANNEL=$(DEFAULT_CHANNEL)
  endif
endif
