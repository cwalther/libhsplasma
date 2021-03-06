/* This file is part of HSPlasma.
 *
 * HSPlasma is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * HSPlasma is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with HSPlasma.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <ResManager/plResManager.h>
#include <ResManager/pdUnifiedTypeMap.h>
#include <Stream/pfPrcHelper.h>
#include <Stream/hsRAMStream.h>
#include <PRP/KeyedObject/hsKeyedObject.h>
#include <list>

static char lnbuf[4096];
static char* lnbuf_ptr = &lnbuf[4096];

plString GetLine(hsStream* S) {
    if (lnbuf_ptr >= &lnbuf[4096]) {
        size_t len = S->size() - S->pos();
        if (len > 4096)
            len = 4096;
        S->read(len, lnbuf);
        if (len < 4096)
            lnbuf[len] = 0;
        lnbuf_ptr = lnbuf;
    }

    char* bp = lnbuf_ptr;
    while (true) {
        if (bp >= &lnbuf[4096]) {
            plString prefix(lnbuf_ptr, bp - lnbuf_ptr);
            lnbuf_ptr = &lnbuf[4096];
            return prefix + GetLine(S);
        } else if (*bp == '\n' || *bp == 0) {
            plString ln(lnbuf_ptr, bp - lnbuf_ptr);
            lnbuf_ptr = bp + 1;
            return ln;
        }
        bp++;
    }

    // Should never get here...
    return plString();
}

void DoSearch(hsStream* S, const char* pattern, const char* filename, plKey key) {
    unsigned int ln = 1;
    lnbuf_ptr = &lnbuf[4096];
    while (!S->eof()) {
        plString text = GetLine(S);
        if (strstr(text.cstr(), pattern) != NULL) {
            // Strip initial whitespace
            const char* txtout = text.cstr();
            while (*txtout == ' ' || *txtout == '\t')
                txtout++;
            printf("%s:[%s]%s:%u: %s\n", filename,
                   pdUnifiedTypeMap::ClassName(key->getType()),
                   key->getName().cstr(), ln, txtout);
        }
        ln++;
    }
}

void doHelp(const char* progname) {
    fprintf(stderr, "Usage: %s pattern file1 [file2 [...]]\n\n", progname);
}

int main(int argc, char** argv) {
    if (argc < 3) {
        doHelp(argv[0]);
        return 1;
    }

    const char* pattern = argv[1];
    std::list<const char*> files;
    for (int i=2; i<argc; i++)
        files.push_back(argv[i]);

    plResManager mgr;
    std::list<const char*>::iterator it;
    for (it = files.begin(); it != files.end(); it++) {
        plPageInfo* page;
        try {
            page = mgr.ReadPage(*it);
        } catch (hsException& ex) {
            fprintf(stderr, "Error reading %s: %s\n", *it, ex.what());
            continue;
        } catch (...) {
            fprintf(stderr, "Undefined error reading %s\n", *it);
            continue;
        }

        std::vector<short> types = mgr.getTypes(page->getLocation());
        std::vector<short>::iterator ti;
        for (ti = types.begin(); ti != types.end(); ti++) {
            std::vector<plKey> keys = mgr.getKeys(page->getLocation(), *ti);
            std::vector<plKey>::iterator ki;
            for (ki = keys.begin(); ki != keys.end(); ki++) {
                hsRAMStream prcOut;
                pfPrcHelper prc(&prcOut);
                prc.exclude(pfPrcHelper::kExcludeTextureData);
                prc.exclude(pfPrcHelper::kExcludeVertexData);
                hsKeyedObject* obj = GET_KEY_OBJECT((*ki), hsKeyedObject);
                if (obj == NULL)
                    continue;
                obj->prcWrite(&prc);
                prcOut.rewind();
                DoSearch(&prcOut, pattern, *it, *ki);
            }
        }
        mgr.UnloadPage(page->getLocation());
    }

    return 0;
}
