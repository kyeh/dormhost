class AvailableDay < ActiveRecord::Base
  has_many :room_profile_days
  
end
