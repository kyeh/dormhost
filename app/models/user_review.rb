class UserReview < ActiveRecord::Base
  belongs_to :profile
  
  validates_presence_of :rating, :review
  
  #acts_as_rateable
end
