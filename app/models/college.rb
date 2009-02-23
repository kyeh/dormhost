class College < ActiveRecord::Base
  has_many :rooms
  has_many :profiles
end
