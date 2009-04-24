class RoomReview < ActiveRecord::Base

  #belongs_to :transaction  #This relationship messes up creating a new room review
  
  #acts_as_rateable
  
  validates_presence_of :rating, :review
end
