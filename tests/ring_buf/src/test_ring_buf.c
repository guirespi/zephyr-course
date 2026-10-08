/*
 * Ring Buffer Module - Homework Test Skeleton
 *
 * test_fresh_state is provided as a worked example. Fill in the remaining
 * 7 ZTEST bodies according to TEST_SPEC.md. Stubs call ztest_test_skip()
 * so the binary builds and runs cleanly before each test is implemented.
 *
 * Run:
 *   west twister -T tests/ring_buf -p native_sim
 */

#include <zephyr/ztest.h>
#include <errno.h>

#include "ring_buf.h"

/*
 * Shared before hook: every suite reinitialises the ring buffer with a
 * capacity of 4 so tests start from a clean, known state. Capacity 4 is
 * enough to exercise FIFO order (push 1, 2, 3) and overflow (full at 4).
 */
static void before(void *f)
{
	ARG_UNUSED(f);
	rb_init(4);
}

/*
 * ============================================================================
 * Test Suite: ring_buf_init
 *
 * Initial state and re-initialization behaviour.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_init, NULL, NULL, before, NULL, NULL);

/* PROVIDED — study this test before writing the rest. */
ZTEST(ring_buf_init, test_fresh_state)
{
	zassert_true(rb_is_empty(), "Fresh buffer must be empty");
	zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

ZTEST(ring_buf_init, test_reinit_clears_state)
{
	rb_push(99);
	rb_init(4);
	zassert_true(rb_is_empty(), "Fresh buffer must be empty");
	zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
	zassert_true(rb_is_empty(), "Ring buffer should be empty");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_push_pop
 *
 * Single push/pop round-trip, FIFO order, full error path.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_push_pop, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_push_pop, test_single_push_pop)
{
	/* TODO(l8-task1): rb_push(42), rb_pop(&v) -> v == 42, buffer empty after.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #1.
	 */
	int v = 0;
	int push_val = 42;
	zassert_equal(rb_push(push_val), 0, "Ring buffer push should succeed");
	zassert_equal(rb_pop(&v), 0, "Ring buffer pop should succeed");
	zassert_equal(v, push_val, "The popped value should be the same as the pushed value");
	zassert_true(rb_is_empty(), "Ring buffer should be empty");
	zassert_equal(rb_count(), 0, "Ring buffer count should be 0");
}

ZTEST(ring_buf_push_pop, test_fifo_order)
{
	int v = 0;
	rb_push(1);
	rb_push(2);
	rb_push(3);

	rb_pop(&v);
	zassert_equal(v, 1, "Second popped value should be 1");
	rb_pop(&v);
	zassert_equal(v, 2, "Second popped value should be 2");
	rb_pop(&v);
	zassert_equal(v, 3, "Second popped value should be 3");
	zassert_true(rb_is_empty(), "Ring buffer should be empty");
}

ZTEST(ring_buf_push_pop, test_push_full_returns_enospc)
{
	rb_push(1);
	rb_push(2);
	rb_push(3);
	rb_push(4);

	zassert_equal(rb_push(99), -ENOSPC, "Ring buffer should return ENOSPC");
	zassert_equal(rb_count(), 4, "Ring buffer count should be 4");
	zassert_true(rb_is_full(), "Ring buffer should be full");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_boundaries
 *
 * Peek semantics and NULL-pointer boundary conditions.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_boundaries, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_boundaries, test_peek_does_not_consume)
{
	int v = 0;
	int push_val = 7;
	rb_push(push_val);
	zassert_equal(rb_peek(&v), 0, "Ring buffer peek should succeed");
	zassert_equal(v, push_val, "Peek should not consume");
	zassert_equal(rb_peek(&v), 0, "Ring buffer peek should succeed");
	zassert_equal(v, push_val, "Peek should not consume");
	zassert_equal(rb_count(), 1, "Ring buffer count should be 1");
}

ZTEST(ring_buf_boundaries, test_pop_null_returns_einval)
{
	zassert_equal(rb_pop(NULL), -EINVAL, "Ring buffer should return EINVAL on Null input pointer");
}

ZTEST(ring_buf_boundaries, test_is_full_after_fill)
{
	zassert_equal(rb_push(1), 0, "Ring buffer push should succeed");
	zassert_equal(rb_push(2), 0, "Ring buffer push should succeed");
	zassert_equal(rb_push(3), 0, "Ring buffer push should succeed");
	zassert_equal(rb_push(4), 0, "Ring buffer push should succeed");
	zassert_true(rb_is_full(), "Ring buffer should be full");
	zassert_equal(rb_count(), 4, "Ring buffer count should be 4");
}
