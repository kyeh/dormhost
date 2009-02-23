class RoomProfileDay < ActiveRecord::Base
  belongs_to :available_day
  belongs_to :room_profile
end
