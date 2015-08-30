#pragma once

#include <list>
#include <map>
#include <memory>

#include "base/not_null.hpp"
#include "geometry/named_quantities.hpp"

namespace principia {

using geometry::Instant;

namespace physics {

// This traits class must export declarations similar to the following:
//
// using TimelineConstIterator = ...;
// static Instant const& time(TimelineConstIterator const it);
//
// TimelineConstIterator must be an STL-like iterator in the timeline of
// Tr4jectory.  |time()| must return the corresponding time.
template<typename Tr4jectory>
struct ForkableTraits;

// This template represents a trajectory which is forkable and iterable.  It
// uses CRTP to achive static polymorphism on the return type of the member
// functions: we want them to return Tr4jectory, not Forkable, so that the
// clients don't have to down_cast.
template<typename Tr4jectory>
class Forkable {
 public:
  // An iterator into the timeline of the trajectory.  Must be STL-like.
  // Beware, if these iterators are invalidated all the guarantees of Forkable
  // are void.
  using TimelineConstIterator =
      typename ForkableTraits<Tr4jectory>::TimelineConstIterator;

  Forkable() = default;

  // Creates a new child trajectory forked at time |time|, and returns it.  The
  // child trajectory shares its data with the current trajectory for times less
  // than or equal to |time|, and is an exact copy of the current trajectory for
  // times greater than |time|.  It may be changed independently from the
  // parent trajectory for any time (strictly) greater than |time|.  The child
  // trajectory is owned by its parent trajectory.  Deleting the parent
  // trajectory deletes all child trajectories.  |time| must be one of the times
  // of this trajectory, and must be at or after the fork time, if any.
  not_null<Tr4jectory*> NewFork(Instant const& time);

  // Deletes the child trajectory denoted by |*fork|, which must be a pointer
  // previously returned by NewFork for this object.  Nulls |*fork|.
  void DeleteFork(not_null<Tr4jectory**> const trajectory);

  // Returns true if this is a root trajectory.
  bool is_root() const;

  // Returns the root trajectory.
  not_null<Tr4jectory const*> root() const;
  not_null<Tr4jectory*> root();

  // Returns the fork time for a nonroot trajectory and null for a root
  // trajectory.
  Instant const* ForkTime() const;  // optional

  // A base class for iterating over the timeline of a trajectory, taking forks
  // into account.
  class Iterator {
   public:
    bool operator==(Iterator const& right) const;
    bool operator!=(Iterator const& right) const;

    Iterator& operator++();
    Iterator& operator--();

    // Returns the point in the timeline that is denoted by this iterator.
    TimelineConstIterator current() const;

   private:
    Iterator() = default;

    //TODO(phl):
    void NormalizeIfEnd();
    void CheckNormalizedIfEnd();

    // |ancestry_| is never empty.  |current_| is an iterator in the timeline
    // for |ancestry_.front()|.  |current_| may be at end.
    TimelineConstIterator current_;
    std::list<not_null<Tr4jectory const*>> ancestry_;  // Pointers not owned.

    template<typename Tr4jectory>
    friend class Forkable;
  };

  Iterator Begin() const;
  Iterator End() const;

  Iterator Find(Instant const& time) const;

  // Constructs an Iterator by wrapping the timeline iterator
  // |position_in_ancestor_timeline| which must be an iterator in the timeline
  // of |ancestor|.  |ancestor| must be an ancestor of this trajectory
  // (including the trajectory itself).
  Iterator Wrap(
      not_null<const Tr4jectory*> const ancestor,
      TimelineConstIterator const position_in_ancestor_timeline) const;

 protected:
  // The API that must be implemented by subclasses.

  // Must return |this| of the proper type
  virtual not_null<Tr4jectory*> that() = 0;
  virtual not_null<Tr4jectory const*> that() const = 0;

  // STL-like operations.
  virtual TimelineConstIterator timeline_begin() const = 0;
  virtual TimelineConstIterator timeline_end() const = 0;
  virtual TimelineConstIterator timeline_find(Instant const& time) const = 0;
  virtual void timeline_insert(TimelineConstIterator begin,
                               TimelineConstIterator end) = 0;
  virtual bool timeline_empty() const = 0;

 private:
  // There may be several forks starting from the same time, hence the multimap.
  using Children = std::multimap<Instant, Tr4jectory>;

  // Null for a root.
  Tr4jectory* parent_ = nullptr;

  // TODO(phl): The following two iterators should be optional because we don't
  // really have a good value for roots.

  // This iterator is never at |end()|.
  typename Children::const_iterator position_in_parent_children_;

  // This iterator is at |end()| if the fork time is not in the parent timeline,
  // i.e. is the parent timeline's own fork time.
  TimelineConstIterator position_in_parent_timeline_;

  Children children_;
};

}  // namespace physics
}  // namespace principia

#include "physics/forkable_body.hpp"
