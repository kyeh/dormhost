class RoomProfile < ActiveRecord::Base
  has_one :room
  belongs_to :level
  has_many :room_images
  has_many :room_profile_days
  
end
