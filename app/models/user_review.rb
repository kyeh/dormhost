class UserReview < ActiveRecord::Base
  belongs_to :profile
  
  acts_as_rateable
end
