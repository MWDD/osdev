// ------------------------------------------------------------------------------------------------
// net.c
// ------------------------------------------------------------------------------------------------

#include "net.h"
#include "arp.h"
#include "dhcp.h"
#include "loopback.h"

// ------------------------------------------------------------------------------------------------
u8 net_trace = 0;

// ------------------------------------------------------------------------------------------------
void net_init()
{
    loopback_init();
    arp_init();

    // Initialize interfaces
    Link* it = g_net_intf_list.next;
    Link* end = &g_net_intf_list;

    while (it != end)
    {
        Net_Intf* intf = link_data(it, Net_Intf, link);

        // Check if interface needs IP address dynamically assigned
        if (!intf->ip_addr.u.bits)
        {
            dhcp_discover(intf);
        }

        it = it->next;
    }
}

// ------------------------------------------------------------------------------------------------
void net_poll()
{
    // Poll interfaces
    Link* it = g_net_intf_list.next;
    Link* end = &g_net_intf_list;

    while (it != end)
    {
        Net_Intf* intf = link_data(it, Net_Intf, link);

        intf->poll(intf);

        it = it->next;
    }
}
