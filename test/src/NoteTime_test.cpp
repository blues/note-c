/*!
 * @file NoteTime_test.cpp
 *
 * Written by the Blues Inc. team.
 *
 * Copyright (c) 2023 Blues Inc. MIT License. Use of this source code is
 * governed by licenses granted by the copyright holder including that found in
 * the
 * <a href="https://github.com/blues/note-c/blob/master/LICENSE">LICENSE</a>
 * file.
 *
 */

#include <catch2/catch_test_macros.hpp>
#include <fff.h>

#include "n_lib.h"
#include "time_mocks.h"

DEFINE_FFF_GLOBALS
FAKE_VALUE_FUNC(J *, NoteNewRequest, const char *)
FAKE_VALUE_FUNC(J *, NoteRequestResponse, J *)
FAKE_VALUE_FUNC(uint32_t, NoteGetMs)

namespace
{

// Controllable millisecond clock for timer-gated NoteTimeST tests.
static uint32_t mockMs;

uint32_t NoteGetMsMock(void)
{
    return mockMs;
}

// Fresh card.time response each call; NoteTimeST deletes the object it receives.
J *NoteRequestResponseValidCardTime(J *req)
{
    (void)req;

    J *resp = JCreateObject();
    if (resp == NULL) {
        return NULL;
    }

    JAddNumberToObject(resp, "time", 1599769214);
    JAddStringToObject(resp, "area", "Beverly, MA");
    JAddStringToObject(resp, "zone", "CDT,America/New York");
    JAddNumberToObject(resp, "minutes", -300);
    JAddStringToObject(resp, "country", "US");

    return resp;
}

SCENARIO("NoteTime")
{
    NoteSetFnDefault(malloc, free, NULL, NULL);

    SECTION("No response to card.time") {
        NoteRequestResponse_fake.return_val = NULL;
        NoteGetMs_fake.return_val = 1000;

        // Unable to get time from Notecard, so we should get back the
        // seconds since boot.
        CHECK(NoteTime() == 1);
    }

    SECTION("card.time errors") {
        J* resp = JCreateObject();
        REQUIRE(resp != NULL);
        NoteRequestResponse_fake.return_val = resp;
        NoteGetMs_fake.return_val = 1000;

        SECTION("err field present") {
            JAddStringToObject(resp, "err", "an error");
        }

        SECTION("No time field") {
        }

        CHECK(NoteTime() == 1);
    }

    SECTION("card.time valid") {
        NoteGetMs_fake.custom_fake = NoteGetMsIncrement;
        char respRaw[] =
            "{ \
          \"time\": 1599769214, \
          \"area\": \"Beverly, MA\", \
          \"zone\": \"CDT,America/New York\", \
          \"minutes\": -300, \
          \"lat\": 42.5776, \
          \"lon\": -70.87134, \
          \"country\": \"US\" \
        }";
        J *resp = JParse(respRaw);
        REQUIRE(resp != NULL);
        NoteRequestResponse_fake.return_val = resp;

        JTIME time = NoteTime();
        // The returned time should be less than what's in the JSON response
        // from the Notecard because NoteTimeST subtracts the time it took to
        // fetch the time.
        CHECK((time > 0 && time < 1599769214));
    }

    SECTION("Millisecond rollover") {
        JTIME baseTime = 1679335667;
        uint32_t baseTimeSetAtMs = 1000;
        uint32_t rolloverMs = 500;
        uint32_t getMsRetVals[] = {baseTimeSetAtMs, rolloverMs};
        SET_RETURN_SEQ(NoteGetMs, getMsRetVals, 2);

        // Set the time manually so that the base time is non-zero.
        NoteTimeSet(baseTime, 0, NULL, NULL, NULL);
        // Compute the new base time, taking into account the millisecond
        // rollover.
        JTIME newBaseTime = baseTime + (0x100000000LL + rolloverMs -
                                        baseTimeSetAtMs) / 1000;
        CHECK(NoteTime() == newBaseTime);
    }

    SECTION("Refresh timer alone re-fetches card.time via NoteTimeST") {
        // Clear any prior cached/manual time so auto Notecard fetch is enabled.
        NoteTimeSet(0, 0, NULL, NULL, NULL);
        // Match the SDK default (five minutes). Using the public setter keeps
        // the period explicit even if static state was polluted by earlier
        // sections.
        const uint32_t refreshMins = 5;
        const uint32_t refreshMs = refreshMins * 60 * 1000;
        NoteTimeRefreshMins(refreshMins);

        mockMs = 1000;
        NoteGetMs_fake.custom_fake = NoteGetMsMock;
        NoteRequestResponse_fake.custom_fake = NoteRequestResponseValidCardTime;

        // First NoteTimeST: no base time yet, so one card.time transaction.
        CHECK(NoteTimeST() > 0);
        CHECK(NoteRequestResponse_fake.call_count == 1);

        // Still inside the refresh window: cached time is returned without a
        // new Notecard transaction. NoteTimeST (not NoteTime) is required so
        // the suppression timer is not force-cleared.
        mockMs = 1000 + (refreshMs / 2);
        CHECK(NoteTimeST() > 0);
        CHECK(NoteRequestResponse_fake.call_count == 1);

        // At/after the refresh period, NoteTimeST should issue card.time again.
        mockMs = 1000 + refreshMs;
        CHECK(NoteTimeST() > 0);
        CHECK(NoteRequestResponse_fake.call_count == 2);
    }

    RESET_FAKE(NoteNewRequest);
    RESET_FAKE(NoteRequestResponse);
    RESET_FAKE(NoteGetMs);
}

}


