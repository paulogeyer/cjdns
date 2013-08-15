/* vim: set expandtab ts=4 sw=4: */
/*
 * You may redistribute this program and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "admin/AdminClient.h"
#include "benc/Dict.h"
#include "benc/String.h"
#include "benc/Int.h"
#include "memory/MallocAllocator.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/log/Log.h"
#include "util/log/FileWriterLog.h"
#include "util/platform/Sockaddr.h"
#include "util/Assert.h"

int main(int argc, char** argv)
{
    struct Allocator* alloc = MallocAllocator_new(1<<20);
    struct EventBase* eventBase = EventBase_new(alloc);
    struct Log* logger = FileWriterLog_new(stderr, alloc);


    // admin password to the router
    String* adminPass = String_new("set this to your cjdns admin password", alloc);

    // get the number of bytes of memory allocated by cjdns
    String* funcName = String_new("memory", alloc);

    // no arguments
    Dict* args = Dict_new(alloc);

    // The running cjdroute engine
    struct Sockaddr_storage ss;
    Assert_true(!Sockaddr_parse("127.0.0.1:11234", &ss));
    struct Sockaddr* addr = &ss.addr;


    //////////////////

    struct AdminClient* client = AdminClient_new(addr, adminPass, eventBase, logger, alloc);

    struct AdminClient_Result* result = AdminClient_rpcCall(funcName, args, client, alloc);

    if (result->err) {
        Log_info(logger, "Error [%s]", AdminClient_errorString(result->err));
    } else {
        int64_t* bytes = Dict_getInt(result->responseDict, String_new("bytes", alloc));
        if (!bytes) {
            Log_info(logger, "Error bytes not in response");
        } else {
            Log_info(logger, "cjdns is using [%d] bytes of memory", (int) (*bytes));
        }
    }

    // This should be possible but there's a race condition when freeing the eventBase.
    //Allocator_free(alloc);
}
